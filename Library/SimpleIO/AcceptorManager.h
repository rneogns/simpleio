#pragma once

class IONotifier;
class SessionManager;
class IDispatcher;
class Acceptor;

class AcceptorManager
{
	typedef std::list<Acceptor* > AcceptorList;

private:
	AcceptorList _acceptors;

public:
	AcceptorManager();
	~AcceptorManager();

	Acceptor* CreateAcceptor(IONotifier* ioNoti, SessionManager* ssMgr, IDispatcher* disp);
};

