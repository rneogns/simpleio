#pragma once

#include "GlobalDefines.h"

class Session;

class User
{
private:
	Session* session;
	unsigned int userID;
	char userName[MAX_USER_NAME];

public:
	User(Session* s);
	~User();

	void Clear();

	Session* GetSession();
	unsigned int GetSessionID();
	unsigned int GetUserID();
	char* GetUserName();
};
