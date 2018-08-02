#pragma once

#include "Thread.h"

class SimpleIO;
class IRedisDispatcher;

class RedisThread : public Thread
{
private:
	SimpleIO* _simpleio;
	IRedisDispatcher* _redisDispatcher;

public:
	RedisThread(SimpleIO* simpleio);
	void SetRedisDispatcher(IRedisDispatcher* disp);

	void* Process();
};

