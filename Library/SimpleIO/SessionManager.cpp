#include "Precompiled.h"
#include "SessionManager.h"
#include "Session.h"
#include "SimpleIO.h"
#include "IONotifier.h"

SessionManager::SessionManager(SimpleIO* simpleio)
	: _simpleio(simpleio)
	, _session_pool(MAX_SESSION_COUNT)
{
}

// Worker Threadø°º≠ »£√‚µ 
Session* SessionManager::CreateSession(boost::asio::io_service* ios, IDispatcher* disp)
{
	std::lock_guard<std::mutex> guard(_lock);

	Session* s = _session_pool.construct(ios);
	
	s->SetSessionManager(this);
	s->SetMessageQueue(_simpleio->GetMessageQueue());
	s->SetDispatcher(disp);

	_session_set.insert(s);
	
	return s;
}

// Dispatch Threadø°º≠ »£√‚µ 
bool SessionManager::IsValid(Session* s)
{
	std::lock_guard<std::mutex> guard(_lock);

	auto itr = _session_set.find(s);
	if (itr != _session_set.end())
	{
		return true;
	}

	return false;
}

void SessionManager::DestroySession(Session* s)
{
	std::lock_guard<std::mutex> guard(_lock);

	if (s == nullptr)
		return;

	s->Clear();
	
	_session_set.erase(s);

	if (!_session_pool.is_from(s))
	{
		LOG_CORE(L"session pointer is something wrong");
		return;
	}
	_session_pool.destroy(s);
}
