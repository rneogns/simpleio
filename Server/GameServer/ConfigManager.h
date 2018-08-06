#pragma once

#include "GlobalDefines.h"

struct NetworkConfig
{
	string ip;
	unsigned short port;

	NetworkConfig()
		: ip(""), port(0)
	{};
};

struct LogConfig
{
	string logPath;

	LogConfig()
		: logPath("")
	{}
};

struct MysqlConfig
{
	string ip;
	unsigned short port;
	string user;
	string passwd;
	string dbname;

	MysqlConfig()
		: ip(""), port(0), user(""), passwd(""), dbname("")
	{};
};

struct RedisConfig
{
	string ip;
	unsigned short port;

	RedisConfig()
		: ip(""), port(0)
	{};
};

class ConfigManager
{
public:
	ConfigManager();

	NetworkConfig networkConfig;
	LogConfig logConfig;
	MysqlConfig mysqlConfig[MAX_DB_HANDLE];
	RedisConfig redisConfig[MAX_REDIS_HANDLE];

	bool ReadConfig(char* fullpath);
};
