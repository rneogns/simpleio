#include "Precompiled.h"
#include "ByteBuffer.h"
#include "Session.h"
#include "SessionManager.h"
#include "Dispatcher.h"
#include "Stream.h"
#include "PacketHeader.h"

Session::Session(boost::asio::io_service* ios)
	: _messageQueue(nullptr)
	, _sessionManager(nullptr)
	, _dispatcher(nullptr)
	, _recvBuffer(new ByteBuffer(MAX_RECV_BUFFER))
	, _sendBuffer(new ByteBuffer(MAX_SEND_BUFFER))
	, _socket(new boost::asio::ip::tcp::socket(*ios))
{
}

Session::~Session()
{
	Clear();

	delete _recvBuffer;
	delete _sendBuffer;
}

boost::asio::ip::tcp::socket* Session::GetSocket()
{
	return _socket;
}

void Session::Clear()
{
	_dispatcher = nullptr;
}

void Session::SetMessageQueue(MessageQueue* mgr)
{
	_messageQueue = mgr;
}

void Session::SetSessionManager(SessionManager* mgr)
{
	_sessionManager = mgr;
}

void Session::SetDispatcher(IDispatcher* disp)
{
	_dispatcher = disp;
}

void Session::OnConnect()
{
	_dispatcher->OnConnected(this);
}

void Session::PutStream(Stream* s)
{
	s->SetSession(this);

	_messageQueue->Push(QueueType::PACKET_Q, s);
}

void Session::Dispatch(Packet* packet)
{
	_dispatcher->Dispatch(this, packet);
}

void Session::CloseSession()
{
	_dispatcher->OnDisconnected(this, errno);
	//closesocket(socket.GetFD());
	_socket->close();
	_sessionManager->DestroySession(this);
}

void Session::Send(Packet& packet)
{
	packet.EndPacket();

	ByteBuffer* buffer = packet.GetByteBuffer();

	// send 버퍼로 데이터를 copy한다
	_sendBuffer->Append(buffer);

	// send io를 호출한다
	StartSend();
}

void Session::Send(PacketHeader* fixed_packet)
{
	Packet packet;
	packet.PutBinary((char*)fixed_packet, fixed_packet->packetSize);
	packet.EndPacket();

	// send 버퍼로 데이터를 copy한다
	_sendBuffer->Append(packet.GetByteBuffer());

	// send io를 호출한다
	StartSend();
}


void Session::StartRecv()
{
	_socket->async_read_some(boost::asio::buffer(_recvBuffer->WritePtr(), MAX_RECV_BUFFER),
		boost::bind(&Session::OnRecv, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
	);
}

void Session::OnRecv(const boost::system::error_code& ec, size_t recv_bytes)
{
	// Recv 작업이 실패한 경우 세션을 끊어준다.
	if (ec.value() != 0)
	{
		LOG_CORE("session receive job error occurred. error number (%d)", ec.value());
		CloseSession();
		return;
	}

	//  상대방이 끊을 경우에도 접속종료 처리를 한다
	if (recv_bytes == 0)
	{
		LOG_CORE("peer has disconnected");
		CloseSession();
		return;
	}

	// Recv 작업이 성공하면 버퍼 위치를 옮겨준다 (데이터는 채워져서 들어오기 때문에)
	_recvBuffer->Write((int)recv_bytes);

	if (_dispatcher->Parsing(this, _recvBuffer) == false)
	{
		// 파싱이 실패한 경우에 접속종료 처리를 한다
		LOG_CORE("packet parsing error occurred. session will close");
		CloseSession();
		return;
	}

	// Recv IO 작업을 다시 시작한다
	StartRecv();
}

void Session::StartSend()
{
	boost::asio::async_write((*_socket), boost::asio::buffer(_sendBuffer->ReadPtr(), (size_t)_sendBuffer->Length())
									, boost::bind(&Session::OnSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Session::OnSend(const boost::system::error_code& ec, size_t send_bytes)
{
	LOG_CORE("send bytes: %d\n", send_bytes);
}
