#include "Precompiled.h"
#include "DBThread.h"
#include "SimpleIO.h"
#include "MessageQueue.h"
#include "Stream.h"
#include "Dispatcher.h"

DBThread::DBThread(SimpleIO* simpleio)
	: _simpleio(simpleio)
	, _dbDispatcher(nullptr)
{
}


void DBThread::SetDBDispatcher(IDBDispatcher* disp)
{
	_dbDispatcher = disp;
}

void* DBThread::Process()
{
	MessageQueue* msgQueue = _simpleio->GetMessageQueue();

	while(_simpleio->isRunning.load())
	{
		if (msgQueue->Empty(QueueType::DB_Q))
			continue;

		Stream* stream = msgQueue->Pop(QueueType::DB_Q);

		if (_dbDispatcher)
			_dbDispatcher->Dispatch((InterMsg*)stream);
	}

	return nullptr;
}
