#pragma once

#include "Thread.h"

class SimpleIO;
class IInterDispatcher;
class ITimerDispatcher;

class DispatchThread : public Thread
{
private:
	SimpleIO* _simpleio;
	IInterDispatcher* _interDispatcher;
	ITimerDispatcher* _timerDispatcher;

public:
	DispatchThread(SimpleIO* simpleio);
	void SetInterDispatcher(IInterDispatcher* disp);
	void SetTimerDispatcher(ITimerDispatcher* disp);

	void* Process();
};

