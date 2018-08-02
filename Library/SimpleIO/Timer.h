#pragma once

class Timer
{
public:
	unsigned int timer_id;		// Ÿ�̸� ID (����ġ��)
	void* object;				// Ÿ�̸� ������ ó���ϴµ� �ʿ��� ��� ������Ʈ�� �Ѱ��ش�. dispatcher���� ĳ�����Ͽ� ���
	unsigned int interval;		// Ÿ�̸Ӱ� �����ϴ� �ֱ�
	bool repeat;				// �ݺ����� (true�� ��ӹݺ�, false�� �ѹ� ����ǰ� ����)
	bool enable;				// Ÿ�̸Ӱ� ��ȿ�Ѱ�? true�̸� ���, false�̸� Q���� ó������ �ʰ� ������
	time_t active_time;			// Ÿ�̸Ӱ� �۵� �� �ð� (Q�� �� �� ����ð��� interval�� ���� ������ �ڵ����� ����)

public:
	Timer();
	Timer(unsigned int id, unsigned int interval, void* object, bool repeat);

	bool operator < (const Timer& rhs) const;
};

#define TIMER_ONCE(id, interval, object)			\
	Timer timer_##id(id, interval, object, false);	\
	TIMER_QUEUE()->Push(timer_##id);

#define TIMER_REPEAT(id, interval, object)			\
	Timer timer_##id(id, interval, object, true);	\
	TIMER_QUEUE()->Push(timer_##id);
	