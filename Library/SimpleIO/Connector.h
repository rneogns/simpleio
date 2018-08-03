#pragma once

#define MAX_ACCEPT_BUFFER 1024

class IONotifier;
class Session;
class SessionManager;
class IDispatcher;

class Connector
{
private:
	IONotifier*		_ioNotifier;
	SessionManager* _sessionMgr;
	IDispatcher*	_dispatcher;

public:
	Connector(IONotifier* ioMux, SessionManager* ssMgr, IDispatcher* disp);
	
	bool Connect(const char* ip, unsigned short port);

private:
	void OnConnected(Session* session, const boost::system::error_code& ec);
};

