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

bool Connector::Connect(const wchar_t* ip, unsigned short port)
{
	boost::system::error_code connect_error;
	boost::asio::ip::tcp::endpoint ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(W2S(ip)), port);

	// ���ο� ������ �����
	Session* session = _sessionMgr->CreateSession(_ioNotifier->get_io_service(), _dispatcher);

	// �񵿱� Connect �۾��� �����Ѵ�
	session->GetSocket()->async_connect(ep, boost::bind(&Connector::OnConnected, this, session, boost::asio::placeholders::error));

	return true;
}

void Connector::OnConnected(Session* session, const boost::system::error_code& ec)
{
	if (ec.value() != 0)
	{
		LOG_CORE(L"Connector async_connect job error occurred. error number (%d)", ec.value());
	}
	else
	{
		session->StartRecv();
	}
}