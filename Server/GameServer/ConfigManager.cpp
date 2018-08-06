#include "Precompiled.h"
#include "ConfigManager.h"
#include "ConfigReader.h"
#include "Application.h"

ConfigManager::ConfigManager()
{
}

bool ConfigManager::ReadConfig(char* fullpath)
{
	ConfigReader serverConf;
	serverConf.Read(fullpath);

	// 기본서버 설정을 읽는다
	networkConfig.ip = serverConf.Get<string>("NETWORK.ip", "127.0.0.1");
	networkConfig.port = serverConf.Get<int>("NETWORK.port", 5001);

	// 로그 설정을 읽는다
	logConfig.logPath = serverConf.Get<string>("LOG.log_path", "./log");

	// Mysql DB 설정을 읽는다
	mysqlConfig[ACCOUNT_DB].ip = serverConf.Get<string>("MYSQL.account_db_ip", "52.79.154.180");
	mysqlConfig[ACCOUNT_DB].port = serverConf.Get<int>("MYSQL.account_db_port", 3306);
	mysqlConfig[ACCOUNT_DB].user = serverConf.Get<string>("MYSQL.account_db_user", "admin");
	mysqlConfig[ACCOUNT_DB].passwd = serverConf.Get<string>("MYSQL.account_db_passwd", "tjqjxla!1");
	mysqlConfig[ACCOUNT_DB].dbname = serverConf.Get<string>("MYSQL.account_db_name", "account");

	mysqlConfig[GAME_DB].ip = serverConf.Get<string>("MYSQL.game_db_ip", "52.79.154.180");
	mysqlConfig[GAME_DB].port = serverConf.Get<int>("MYSQL.game_db_port", 3306);
	mysqlConfig[GAME_DB].user = serverConf.Get<string>("MYSQL.game_db_user", "admin");
	mysqlConfig[GAME_DB].passwd = serverConf.Get<string>("MYSQL.game_db_passwd", "tjqjxla!1");
	mysqlConfig[GAME_DB].dbname = serverConf.Get<string>("MYSQL.game_db_name", "account");

	// Redis 설정을 읽는다
	redisConfig[MASTER_REDIS].ip = serverConf.Get<string>("master_redis_ip", "52.79.155.216");
	redisConfig[MASTER_REDIS].port = serverConf.Get<int>("master_redis_port", 6379);

	return true;
}
