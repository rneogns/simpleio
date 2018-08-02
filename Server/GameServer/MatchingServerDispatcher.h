#pragma once

#include "Dispatcher.h"

class MatchingServerDispatcher : public IDispatcher
{
public:
	MatchingServerDispatcher();
	virtual ~MatchingServerDispatcher() override;

	virtual void OnConnected(Session* session) override;
	virtual void OnDisconnected(Session* session, int err) override;

	static int DISPATCHER_LOGIN_REQUEST(Session* session, Packet* packet);
};

