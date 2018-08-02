#include "Precompiled.h"
#include "WorkerThread.h"
#include "SimpleIO.h"
#include "IONotifier.h"

WorkerThread::WorkerThread()
	: _simpleio(nullptr)
{
}

WorkerThread::WorkerThread(SimpleIO* simpleio)
	: _simpleio(simpleio)
{
}

void WorkerThread::SetNetEngine(SimpleIO* simpleio)
{
	_simpleio = simpleio;
}

void* WorkerThread::Process()
{
	while(_simpleio->isRunning.load())
	{
		_simpleio->GetIONotifier()->run();
	}

	return nullptr;
}
