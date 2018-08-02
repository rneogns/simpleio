#pragma once

#include "Thread.h"

class SimpleIO;
class IDBDispatcher;

class DBThread : public Thread
{
private:
	SimpleIO* _simpleio;
	IDBDispatcher* _dbDispatcher;

public:
	DBThread(SimpleIO* simpleio);
	void SetDBDispatcher(IDBDispatcher* disp);

	void* Process();
};

