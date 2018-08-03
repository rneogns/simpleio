#include "Precompiled.h"
#include "Connector.h"
#include "IONotifier.h"
#include "Session.h"
#include "SessionManager.h"
#include "StringUtil.h"

Connector::Connector(IONotifier* ioNotifier, SessionManager* ssMgr, IDispatcher* disp)
	: _ioNotifier(ioNotifier)
	, _sessionMgr(ssMgr)
	, _dispatcher(disp)
{
}

bool Connector::Connect(const char* ip, unsigned short port)
{
	boost::system::error_code connect_error;
	boost::asio::ip::tcp::endpoint ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port);

	// 새로운 세션을 만들고
	Session* session = _sessionMgr->CreateSession(_ioNotifier->get_io_service(), _dispatcher);

	// 비동기 Connect 작업을 시작한다
	session->GetSocket()->async_connect(ep, boost::bind(&Connector::OnConnected, this, session, boost::asio::placeholders::error));

	return true;
}

void Connector::OnConnected(Session* session, const boost::system::error_code& ec)
{
	if (ec.value() != 0)
	{
		LOG_CORE("Connector async_connect job error occurred. error number (%d)", ec.value());
	}
	else
	{
		session->StartRecv();
	}
}