#pragma once

#include <boost/asio.hpp>
#include "MemoryPool.h"

class MessageQueue;
class SessionManager;
class IDispatcher;
class ByteBuffer;
class Packet;
class Stream;
struct PacketHeader;

class Session 
//	: public MemoryPool<Session, MAX_SESSION_COUNT>
{
private:
	MessageQueue* _messageQueue;
	SessionManager* _sessionManager;
	IDispatcher* _dispatcher;
	ByteBuffer* _recvBuffer;
	ByteBuffer* _sendBuffer;
	
	boost::asio::ip::tcp::socket* _socket;

public:
	Session(boost::asio::io_service* ios);
	~Session();

	boost::asio::ip::tcp::socket* GetSocket();
	void Clear();
	void SetSessionManager(SessionManager* mgr);
	void SetMessageQueue(MessageQueue* mgr);
	void SetDispatcher(IDispatcher* disp);
	void OnConnect();
	void Dispatch(Packet* packet);
	void CloseSession();
	void PutStream(Stream* s);
	void Send(Packet& packet);
	void Send(PacketHeader* packet);

	void StartRecv();
	void StartSend();

	void OnRecv(const boost::system::error_code& ec, size_t recv_bytes);
	void OnSend(const boost::system::error_code& ec, size_t send_bytes);
};

