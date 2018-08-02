#pragma once

/*
  ____                      _       _   _      _   
/ ___|  __ _ _ __ ___   __| | __ _| \ | | ___| |_ 
\___ \ / _` | '_ ` _ \ / _` |/ _` |  \| |/ _ \ __|
 ___) | (_| | | | | | | (_| | (_| | |\  |  __/ |_ 
|____/ \__,_|_| |_| |_|\__,_|\__,_|_| \_|\___|\__|
                                                  
since 2017
*/

class IONotifier;
class SessionManager;
class AcceptorManager;
class ConnectorManager;
class DBManager;
class RedisManager;
class WorkerThread;
class DispatchThread;
class DBThread;
class RedisThread;
class IDispatcher;
class IDBDispatcher;
class IInterDispatcher;
class ITimerDispatcher;
class IRedisDispatcher;
class MessageQueue;
class TimerQueue;
class Acceptor;
class Connector;
class Session;

class SimpleIO 
{
private:
	// IO처리 객체
	IONotifier* ioNotifier;
	// 연결된 세션을 관리할 객체
	SessionManager*	sessionManager;
	// listen할 port를 관리할 객체
	AcceptorManager* acceptorManager;
	// connect한 port를 관리할 객체
	ConnectorManager* connectorManager;
	// DB연결을 관리할 객체
	DBManager* dbManager;
	// Redis연결을 관리할 객체
	RedisManager* redisManager;
	// IO를 담당할 스레드
	WorkerThread* workerThread;
	// 패킷처리 (비즈니스로직)를 담당할 스레드
	DispatchThread* dispatchThread;
	// DB처리를 담당할 스레드
	DBThread* dbThread;
	// Redis처리를 담당할 스레드
	RedisThread* redisThread;
	// 패킷(스트림)을 담아놓은 큐 매니저
	MessageQueue* messageQueue;
	// 타이머를 담아놓은 큐
	TimerQueue* timerQueue;

public:
	// 활성화 상태인가?
	std::atomic_bool isRunning;

public:
	SimpleIO();
	~SimpleIO();

	bool Start();
	void Stop();
	void SetRunning(bool flag);
	bool Open(unsigned short port, IDispatcher* disp);
	bool Open(const wchar_t* ip, unsigned short port, IDispatcher* disp);
	bool Connect(const wchar_t* ip, unsigned short port, IDispatcher* disp);
	void SetDBDispatcher(IDBDispatcher* disp);
	void SetInterDispatcher(IInterDispatcher* disp);
	void SetTimerDispatcher(ITimerDispatcher* disp);
	void SetRedisDispatcher(IRedisDispatcher* disp);

	SessionManager* GetSessionManager();
	IONotifier* GetIONotifier();
	MessageQueue* GetMessageQueue();
	TimerQueue* GetTimerQueue();
	DBManager* GetDBManager();
	RedisManager* GetRedisManager();

private:
	bool Init();
};

