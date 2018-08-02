#include "Precompiled.h"
#include "TimerQueue.h"
#include "TimeUtil.h"
#include "Timer.h"

void TimerQueue::Push(Timer& timer)
{
	std::lock_guard<std::mutex> guard(_lock);

	timer.active_time = TimeUtil::CurTime() + timer.interval;
	timer.enable = true;

	timerQueue.push(timer);
}

bool TimerQueue::Pop(Timer& timer)
{
	std::lock_guard<std::mutex> guard(_lock);

	if (timerQueue.empty() == true)
		return false;

	timer = timerQueue.top();
	if (timer.active_time <= TimeUtil::CurTime())
	{
		timerQueue.pop();
		return true;
	}

	return false;
}

size_t TimerQueue::Size()
{
	std::lock_guard<std::mutex> guard(_lock);

	return timerQueue.size();
}

bool TimerQueue::Empty()
{
	std::lock_guard<std::mutex> guard(_lock);

	return timerQueue.empty();
}