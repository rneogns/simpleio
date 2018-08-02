#pragma once

enum DB_HANDLE
{
	ACCOUNT_DB,
	GAME_DB,	
	MAX_DB_HANDLE,
};

enum REDIS_HANDLE
{
	MASTER_REDIS,
	MAX_REDIS_HANDLE
};

#define MAX_USER_NAME 32