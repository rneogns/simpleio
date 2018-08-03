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

	bool Open(const char* ip, unsigned int port);
	void Close();
};