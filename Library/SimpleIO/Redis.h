#pragma once

#include <hiredis.h>

class Redis
{
private:

	redisContext* ctx;					// ���� �ڵ�
	redisReply* reply;					// ���� ��� ����

public:
	Redis();
	~Redis();

	bool Open(const wchar_t* ip, unsigned int port);
	void Close();
};