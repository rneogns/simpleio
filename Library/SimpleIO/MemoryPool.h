#pragma once

#include <deque>

template < typename T, size_t N >
class Pool
{
private:
	vector< T* > blocks;		// �Ҵ�� �޸� ��ü �޸� ���� ������ �ִ� ����
	deque< T* > unused;			// �� �� ������� ���� �޸� ���� ������ �ִ� ����
	size_t alloc_count;			// �Ҵ��� ���� (ó���� �����ְ�, �� ����ϸ� �� ����ŭ ����Ͽ� �Ҵ�)
	unsigned int alloc_index;	// ���ݱ��� �Ҵ�� �޸� �� ��ȣ (0���� ������� ����)
	std::mutex mtx_lock;

public:
	Pool()
		: alloc_count(N)
		, alloc_index(0)
	{}

	~Pool()
	{
		for (auto it = blocks.begin(); it != blocks.end(); ++it)
			delete[] (*it);

		blocks.clear();
		unused.clear();
	}

	T* Acquire(size_t size)
	{
		std::lock_guard<std::mutex> guard(mtx_lock);

		if (unused.empty())
		{
			T* block = new T[alloc_count];
			blocks.push_back(block);

			for (int i = 0; i < alloc_count; ++i)
			{
				// 0���� ������� �ʱ� ���� 1������ �Ҵ��Ѵ�.
				// push_front�� �о�־� unused�� [1], [2], [3], [4] ... �̷������� ������ �ְ� �Ѵ�
				block[i].index = ++alloc_index;
				unused.push_front(&block[i]);
			}
		}

		// ��û�ÿ��� ���� �չ����� �����ش�
		T* p = unused.front();
		unused.pop_front();

		return p;
	}

	void Release(void* p)
	{
		std::lock_guard<std::mutex> guard(mtx_lock);

		if (p == nullptr)
			return;

		T* cast = static_cast<T*>(p);

		// Ǯ�� �����ٶ� ����� ��ü�� ���� ���ʿ� �־��ش� 
		// (���� ������ ���߷���, Ȥ�ó� ���ǰ��� �ٸ� ������� �۾��� ������ �����Ǵ� ����, �ٸ� ������ �� ���ǹ�ȣ�� ����ϴ� ��찡 ����� �� �����ϰ���)
		// �� �˻��ϴ� ���� �°����� Ȥ�ó�...
		unused.push_back(cast);
	}
};

// �޸�Ǯ ��ü�� ������� �ϴ� ��ü�� ����Ͽ� ����Ѵ�
// ����� ������ TŸ���� static Pool��ü�� ���������
// new, delete Ű���带 ������ �߱� ������, �ش� ��ü�� new, delete �Ҷ����� pool���� Acquire, Release�� �ҷ����� �ȴ�
template < typename T, size_t N >
class MemoryPool
{
private:
	friend class Pool< T, N >;
	static Pool< T, N > pool;
	unsigned int index;

public:
	MemoryPool()
		: index(0)
	{};

	static void* operator new(size_t size)
	{
		return pool.Acquire(size);
	}

	static void operator delete(void* p)
	{
		return pool.Release(p);
	}

	unsigned int GetID()
	{
		return index;
	}
};

template <typename T, size_t N>
Pool<T, N> MemoryPool<T, N>::pool;