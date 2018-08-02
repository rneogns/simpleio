#pragma once

#include "Timer.h"
#include <queue>

class TimerQueue
{
private:
	priority_queue<Timer> timerQueue;
	std::mutex _lock;

public:
	void Push(Timer& timer);
	bool Pop(Timer& timer);
	size_t Size();
	bool Empty();
};

