#pragma once

class IONotifier;
class SessionManager;
class IDispatcher;
class Connector;

class ConnectorManager
{
	typedef list<Connector* > ConnectorList;

private:
	ConnectorList _connectors;

public:
	ConnectorManager();
	~ConnectorManager();

	Connector* CreateConnector(IONotifier* ioNoti, SessionManager* ssMgr, IDispatcher* disp);
};

