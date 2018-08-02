#include "Precompiled.h"
#include "RedisThread.h"
#include "SimpleIO.h"
#include "MessageQueue.h"
#include "Stream.h"
#include "Dispatcher.h"

RedisThread::RedisThread(SimpleIO* simpleio)
	: _simpleio(simpleio)
	, _redisDispatcher(nullptr)
{
}


void RedisThread::SetRedisDispatcher(IRedisDispatcher* disp)
{
	_redisDispatcher = disp;
}

void* RedisThread::Process()
{
	MessageQueue* msgQueue = _simpleio->GetMessageQueue();

	while(_simpleio->isRunning.load())
	{
		if (msgQueue->Empty(QueueType::REDIS_Q))
			continue;

		Stream* stream = msgQueue->Pop(QueueType::REDIS_Q);

		if (_redisDispatcher)
			_redisDispatcher->Dispatch((InterMsg*)stream);
	}

	return nullptr;
}
