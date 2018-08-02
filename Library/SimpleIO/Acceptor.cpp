#include "Precompiled.h"
#include "Acceptor.h"
#include "IONotifier.h"
#include "Session.h"
#include "SessionManager.h"
#include "StringUtil.h"

Acceptor::Acceptor(IONotifier* ioNotifier, SessionManager* ssMgr, IDispatcher* disp)
	: _ioNotifier(ioNotifier)
	, _sessionMgr(ssMgr)
	, _dispatcher(disp)
	, _acceptor(new boost::asio::ip::tcp::acceptor(*ioNotifier->get_io_service()))
	, _socket(new boost::asio::ip::tcp::socket(*ioNotifier->get_io_service()))
{
}

bool Acceptor::Open(const wchar_t* ip, unsigned short port)
{
	boost::asio::ip::tcp::endpoint ep;

	if (ip == nullptr)
		ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
	else
		ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(W2S(ip)), port);

	try
	{
		_acceptor->open(ep.protocol());

		_acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

		_acceptor->bind(ep);

		_acceptor->listen();
	}
	catch (boost::system::error_code& ec)
	{
		std::wcout << ec.value() << std::endl;
		return false;
	}

	if (StartAccept() == false)
		return false;

	return true;
}

bool Acceptor::StartAccept()
{
	// 새로운 세션을 만들고
 	Session* session = _sessionMgr->CreateSession(_ioNotifier->get_io_service(), _dispatcher);

	// 비동기 Accept 작업을 시작한다
	_acceptor->async_accept(*session->GetSocket(), boost::bind(&Acceptor::OnAccept, this, session, boost::asio::placeholders::error));

	return true;
}

void Acceptor::OnAccept(Session* session, const boost::system::error_code& ec)
{
	// Accept가 완료되었을 경우, 패킷을 받기위해 해당세션 소켓에 receive io 작업을 시작한다
	session->StartRecv();

	// Accept 작업도 다시 시작한다
	StartAccept();
}
