#pragma once

#include <boost/pool/object_pool.hpp>

class Session;
class IDispatcher;
class SimpleIO;

class SessionManager
{
	typedef boost::object_pool<Session> SessionPool;
	typedef std::unordered_set<Session* > SessionSet;

private:	
	SimpleIO* _simpleio;
	SessionPool _session_pool;
	SessionSet _session_set;
	std::mutex _lock;

public:
	SessionManager(SimpleIO* simpleio);
	Session* CreateSession(boost::asio::io_service* ios, IDispatcher* disp);
	void DestroySession(Session* s);
	bool IsValid(Session* s);	
};

