#include "Precompiled.h"
#include "AcceptorManager.h"
#include "IONotifier.h"
#include "SessionManager.h"
#include "Dispatcher.h"
#include "Acceptor.h"

AcceptorManager::AcceptorManager()
{
}

AcceptorManager::~AcceptorManager()
{
	for (auto& acceptor : _acceptors)
		delete acceptor;

	_acceptors.clear();
}

Acceptor* AcceptorManager::CreateAcceptor(IONotifier* ioNoti, SessionManager* ssMgr, IDispatcher* disp)
{
	Acceptor* acceptor = new Acceptor(ioNoti, ssMgr, disp);

	_acceptors.push_back(acceptor);

	return acceptor;
}

