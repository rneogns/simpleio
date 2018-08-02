#pragma once

#define MAX_ACCEPT_BUFFER 1024

class IONotifier;
class SessionManager;
class IDispatcher;

class Connector
{
private:
	IONotifier* ioMux;
	SessionManager* ssMgr;
	IDispatcher* dispatcher;

public:
	Connector(IONotifier* ioMux, SessionManager* ssMgr, IDispatcher* disp);
	
	bool Connect(const wchar_t* ip, unsigned short port);
};

