#include "Precompiled.h"
#include "Connector.h"
#include "IONotifier.h"
#include "Session.h"
#include "SessionManager.h"
#include "StringUtil.h"

Connector::Connector(IONotifier* ioMux, SessionManager* ssMgr, IDispatcher* disp)
	: ioMux(ioMux)
	, ssMgr(ssMgr)
	, dispatcher(disp)
{
}

bool Connector::Connect(const wchar_t* ip, unsigned short port)
{

	return true;
}