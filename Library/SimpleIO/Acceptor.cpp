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
	// ���ο� ������ �����
 	Session* session = _sessionMgr->CreateSession(_ioNotifier->get_io_service(), _dispatcher);

	// �񵿱� Accept �۾��� �����Ѵ�
	_acceptor->async_accept(*session->GetSocket(), boost::bind(&Acceptor::OnAccept, this, session, boost::asio::placeholders::error));

	return true;
}

void Acceptor::OnAccept(Session* session, const boost::system::error_code& ec)
{
	// Accept�� �Ϸ�Ǿ��� ���, ��Ŷ�� �ޱ����� �ش缼�� ���Ͽ� receive io �۾��� �����Ѵ�
	session->StartRecv();

	// Accept �۾��� �ٽ� �����Ѵ�
	StartAccept();
}
