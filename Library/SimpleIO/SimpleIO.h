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
	// IOó�� ��ü
	IONotifier* ioNotifier;
	// ����� ������ ������ ��ü
	SessionManager*	sessionManager;
	// listen�� port�� ������ ��ü
	AcceptorManager* acceptorManager;
	// connect�� port�� ������ ��ü
	ConnectorManager* connectorManager;
	// DB������ ������ ��ü
	DBManager* dbManager;
	// Redis������ ������ ��ü
	RedisManager* redisManager;
	// IO�� ����� ������
	WorkerThread* workerThread;
	// ��Ŷó�� (����Ͻ�����)�� ����� ������
	DispatchThread* dispatchThread;
	// DBó���� ����� ������
	DBThread* dbThread;
	// Redisó���� ����� ������
	RedisThread* redisThread;
	// ��Ŷ(��Ʈ��)�� ��Ƴ��� ť �Ŵ���
	MessageQueue* messageQueue;
	// Ÿ�̸Ӹ� ��Ƴ��� ť
	TimerQueue* timerQueue;

public:
	// Ȱ��ȭ �����ΰ�?
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

