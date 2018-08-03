#pragma once

#include <boost/asio.hpp>

#define MAX_ACCEPT_BUFFER 1024

class IONotifier;
class Session;
class SessionManager;
class IDispatcher;

class Acceptor
{
private:
	IONotifier* _ioNotifier;
	SessionManager* _sessionMgr;
	IDispatcher* _dispatcher;

	boost::asio::ip::tcp::socket* _socket;
	boost::asio::ip::tcp::acceptor* _acceptor;
	
public:
	Acceptor(IONotifier* ioNotifier, SessionManager* ssMgr, IDispatcher* disp);
	
	bool Open(const char* ip, unsigned short port);

private:
	bool StartAccept();
	void OnAccept(Session* session, const boost::system::error_code& ec);
};

