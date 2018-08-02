#pragma once

class Thread
{
protected:
	std::thread* t;

public:
	Thread();
	virtual ~Thread();
	
	bool Start();
	void Join();

	std::thread::id get_id();

	void thread_func();

protected:

	virtual void* Process() = 0;
};
