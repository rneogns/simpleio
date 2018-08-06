#include "Precompiled.h"
#include "Application.h"
#include "ConfigManager.h"
#include "ClientDispatcher.h"
#include "MatchingServerDispatcher.h"
#include "DBDispatcher.h"
#include "RedisDispatcher.h"
#include "InterDispatcher.h"
#include "Timer.h"
#include "TimerQueue.h"
#include "TimerDispatcher.h"
#include "DBManager.h"
#include "RedisManager.h"
#include "SimpleIO.h"
#include "GameManager.h"
#include "Log.h"

Application::Application()
	: networkEngine(new SimpleIO)
	, configManager(new ConfigManager)
	, clientDispatcher(new ClientDispatcher)
	, matchingServerDispatcher(new MatchingServerDispatcher)
	, dbDispatcher(new DBDispatcher)
	, interDispatcher(new InterDispatcher)
	, timerDispatcher(new TimerDispatcher)
	, gameManager(new GameManager)
	, isRunning(false)
{
}

Application::~Application()
{
	delete gameManager;
	delete timerDispatcher;
	delete interDispatcher;
	delete dbDispatcher;
	delete matchingServerDispatcher;
	delete clientDispatcher;	
	delete configManager;
	delete networkEngine;		
}

Application& Application::GetInstance()
{
	static Application instance;
	return instance;
}

bool Application::Init(int argc, char* argv[])
{
	// 설정 cfg 읽기
	if (!ReadConfig(argc, argv))
	{
		LOG_INFO("ReadConfig error");
		return false;
	}

	// 네트워크 설정
	if (!OpenNetwork())
	{
		LOG_INFO("OpenNetwork error");
		return false;
	}

	// 다른 서버 연결
	if (!ConnectServer())
	{
		LOG_INFO("OpenNetwork error");
		return false;
	}

	// DB 연결
	if (!ConnectDB())
	{
		LOG_INFO("ConnectDB error");
		return false;
	}

	// 레디스 연결
	if (!ConnectRedis())
	{
		LOG_INFO("ConnectRedis error");
		return false;
	}

	// 각종 게임에 필요한 객체들 초기화
	InitGameObjects();

	// 모든 설정과 초기화가 완료되었다면 서버동작을 시작한다
	Start();

	LOG_INFO("Server started at ip: %s, port: %d", CONFIG_MANAGER()->networkConfig.ip.c_str(), CONFIG_MANAGER()->networkConfig.port);

	return true;
}

bool Application::ReadConfig(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Invalid argument count (%d)\n", argc);
		return false;
	}

	if (!configManager->ReadConfig(argv[1]))
	{
		printf("Fail to read config file\n");
		return false;
	}

	// 로그 path설정
	Log::GetInstance().SetLogPath(CONFIG_MANAGER()->logConfig.logPath);

	return true;
}

bool Application::OpenNetwork()
{
	const char* server_ip = CONFIG_MANAGER()->networkConfig.ip.c_str();
	unsigned int server_port = CONFIG_MANAGER()->networkConfig.port;

	if (!networkEngine->Open(server_ip, server_port, clientDispatcher))
	{
		LOG_CRI("Fail to start NetworkEngine! ip: %s, port: %d", server_ip, server_port);
		return false;
	}

	// 포트 2개 여는 테스트코드
	//if (!networkEngine->Open(server_ip, 9002, clientDispatcher))
	//{
	//	LOG_CRI("Fail to start NetworkEngine! ip: %s, port: %d", server_ip, server_port);
	//	return false;
	//}

	// InterDispatcher 설정
	networkEngine->SetInterDispatcher(interDispatcher);

	// TimerDispatcher 설정
	networkEngine->SetTimerDispatcher(timerDispatcher);

	return true;
}

bool Application::ConnectServer()
{
	if (!networkEngine->Connect("127.0.0.1", 31400, matchingServerDispatcher))
	{
		LOG_CRI("Fail to connect! ip: %s, port: %d", "127.0.0.1", 31400);
		return false;
	}

	return true;
}

bool Application::ConnectDB()
{
	for (int i = ACCOUNT_DB; i < MAX_DB_HANDLE; ++i)
	{
		const char* db_ip = CONFIG_MANAGER()->mysqlConfig[i].ip.c_str();
		unsigned int db_port = CONFIG_MANAGER()->mysqlConfig[i].port;
		const char* db_name = CONFIG_MANAGER()->mysqlConfig[i].dbname.c_str();
		const char* db_user = CONFIG_MANAGER()->mysqlConfig[i].user.c_str();
		const char* db_passwd = CONFIG_MANAGER()->mysqlConfig[i].passwd.c_str();

		networkEngine->GetDBManager()->Open(i, db_ip, db_port, db_name, db_user, db_passwd);
		networkEngine->SetDBDispatcher(dbDispatcher);
	}

	return true;
}

bool Application::ConnectRedis()
{
	for (int i = MASTER_REDIS; i < MAX_REDIS_HANDLE; ++i)
	{
		const char* redis_ip = CONFIG_MANAGER()->redisConfig[i].ip.c_str();
		unsigned int redis_port = CONFIG_MANAGER()->redisConfig[i].port;

		networkEngine->GetRedisManager()->Open(i, redis_ip, redis_port);
		networkEngine->SetRedisDispatcher(redisDispatcher);
	}

	return true;
}

bool Application::InitGameObjects()
{
	// 게임매니저 설정
	gameManager->Init();

	return true;
}

void Application::Start()
{
	SetRunning(true);

	networkEngine->Start();
}

void Application::Process()
{
	while (isRunning)
	{
		GAME_MANAGER()->Init();

		getchar();
		isRunning = false;

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void Application::Close()
{
	networkEngine->Stop();
}

void Application::SetRunning(bool flag)
{
	isRunning.store(flag);
}

ConfigManager* Application::GetConfigManager()
{
	return configManager;
}

GameManager* Application::GetGameManager()
{
	return gameManager;
}

SimpleIO* Application::GetNetworkEngine()
{
	return networkEngine;
}
