#include "Precompiled.h"
#include "Thread.h"
#include "Log.h"

Thread::Thread()
	: t(nullptr)
{
}

Thread::~Thread()
{
	t->detach();
}

std::thread::id Thread::get_id()
{
	return t->get_id();
}

void Thread::thread_func()
{
	this->Process();
}

bool Thread::Start()
{
	t = new std::thread(boost::bind(&Thread::thread_func, this));

	return true;
}

void Thread::Join()
{
	if (t->joinable())
		t->join();
}
