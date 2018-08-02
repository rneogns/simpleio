#include "Precompiled.h"
#include "ConnectorManager.h"
#include "IONotifier.h"
#include "SessionManager.h"
#include "Dispatcher.h"
#include "Connector.h"

ConnectorManager::ConnectorManager()
{
}

ConnectorManager::~ConnectorManager()
{
	for (auto& connector : _connectors)
		delete connector;

	_connectors.clear();
}

Connector* ConnectorManager::CreateConnector(IONotifier* ioNoti, SessionManager* ssMgr, IDispatcher* disp)
{
	Connector* connector = new Connector(ioNoti, ssMgr, disp);

	_connectors.push_back(connector);

	return connector;
}

