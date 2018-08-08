#include "Precompiled.h"
#include "Timer.h"

Timer::Timer()
	: timer_id(0)
	, object(nullptr)
	, interval(0)
	, repeat(false)
	, enable(false)
	, active_time(0)
{
}

Timer::Timer(unsigned int id, unsigned int interval, void* object, bool repeat)
	: timer_id(id)
	, object(object)
	, interval(interval)
	, repeat(repeat)
	, enable(false)
	, active_time(0)
{
}

bool Timer::operator < (const Timer& rhs) const
{
	return active_time < rhs.active_time;
}
