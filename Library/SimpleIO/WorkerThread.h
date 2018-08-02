#pragma once

#include "Thread.h"

class SimpleIO;

class WorkerThread : public Thread
{
private:
	SimpleIO* _simpleio;

public:
	WorkerThread();
	WorkerThread(SimpleIO* simpleio);

	void SetNetEngine(SimpleIO* simpleio);
	void* Process();
};

