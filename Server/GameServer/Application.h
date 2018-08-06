#pragma once

#include "GlobalDefines.h"
#include <atomic>

class SimpleIO;
class ConfigManager;
class ClientDispatcher;
class MatchingServerDispatcher;
class DBDispatcher;
class RedisDispatcher;
class InterDispatcher;
class TimerDispatcher;
class GameManager;

class Application
{
private:
	ConfigManager*				configManager;
	SimpleIO*					networkEngine;
	ClientDispatcher*			clientDispatcher;
	MatchingServerDispatcher*	matchingServerDispatcher;
	DBDispatcher*				dbDispatcher;
	RedisDispatcher*			redisDispatcher;
	InterDispatcher*			interDispatcher;
	TimerDispatcher*			timerDispatcher;
	GameManager*				gameManager;

public:
	std::atomic_bool			isRunning;

public:
	Application();
	~Application();

	static Application& GetInstance();

	bool Init(int argc, char* argv[]);
	void Process();
	void Close();

	void SetRunning(bool flag);

	ConfigManager* GetConfigManager();
	GameManager* GetGameManager();
	SimpleIO* GetNetworkEngine();

private:
	bool ReadConfig(int argc, char* argv[]);
	bool OpenNetwork();
	bool ConnectServer();
	bool ConnectDB();
	bool ConnectRedis();
	bool InitGameObjects();
	void Start();

};

#define CONFIG_MANAGER() Application::GetInstance().GetConfigManager()
#define GAME_MANAGER() Application::GetInstance().GetGameManager()
#define NETWORK_ENGINE() Application::GetInstance().GetNetworkEngine()
#define MESSAGE_QUEUE() NETWORK_ENGINE()->GetMessageQueue()
#define TIMER_QUEUE() NETWORK_ENGINE()->GetTimerQueue()
#define DB_MANAGER() NETWORK_ENGINE()->GetDBManager()
#define REDIS_MANAGER() NETWORK_ENGINE()->GetRedisManager()
#define USER_MANAGER() GAME_MANAGER()->GetUserManager()