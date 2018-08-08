#include "Precompiled.h"
#include "SimpleIO.h"
#include "Dispatcher.h"
#include "IONotifier.h"
#include "Session.h"
#include "SessionManager.h"
#include "DBManager.h"
#include "RedisManager.h"
#include "WorkerThread.h"
#include "DispatchThread.h"
#include "DBThread.h"
#include "RedisThread.h"
#include "Dispatcher.h"
#include "TimerQueue.h"
#include "Acceptor.h"
#include "AcceptorManager.h"
#include "Connector.h"
#include "ConnectorManager.h"

SimpleIO::SimpleIO()
	: ioNotifier(new IONotifier)	
	, sessionManager(new SessionManager(this))
	, acceptorManager(new AcceptorManager)
	, connectorManager(new ConnectorManager)
	, dbManager(new DBManager)
	, redisManager(new RedisManager)
	, workerThread(new WorkerThread(this))
	, dispatchThread(new DispatchThread(this))
	, dbThread(new DBThread(this))
	, redisThread(new RedisThread(this))
	, messageQueue(new MessageQueue)
	, timerQueue(new TimerQueue)
	, isRunning(false)
{
	Init();
}

SimpleIO::~SimpleIO()
{
	Stop();

	delete connectorManager;
	delete acceptorManager;
	delete timerQueue;
	delete messageQueue;
	delete redisManager;
	delete dbManager;
	delete sessionManager;
	delete redisThread;
	delete dbThread;
	delete dispatchThread;
	delete workerThread;	
	delete ioNotifier;
}

bool SimpleIO::Init()
{
	return true;
}

SessionManager* SimpleIO::GetSessionManager()
{
	return sessionManager;
}

MessageQueue* SimpleIO::GetMessageQueue()
{
	return messageQueue;
}

TimerQueue* SimpleIO::GetTimerQueue()
{
	return timerQueue;
}

IONotifier* SimpleIO::GetIONotifier()
{
	return ioNotifier;
}

DBManager* SimpleIO::GetDBManager()
{
	return dbManager;
}

RedisManager* SimpleIO::GetRedisManager()
{
	return redisManager;
}

bool SimpleIO::Start()
{
	// 동작플래그를 켜고, 필요한 스레드를 동작시킨다
	SetRunning(true);

	dispatchThread->Start();
	workerThread->Start();	
	dbThread->Start();	
	redisThread->Start();

	return true;
}

void SimpleIO::Stop()
{
	// 동작플래그 OFF
	SetRunning(false);

	// 각 스레드가 종료되기를 기다린다
	workerThread->Join();
	dispatchThread->Join();
	redisThread->Join();
	dbThread->Join();
}

void SimpleIO::SetRunning(bool flag)
{
	isRunning.store(flag);
}

void SimpleIO::SetDBDispatcher(IDBDispatcher* disp)
{
	dbThread->SetDBDispatcher(disp);
}

void SimpleIO::SetInterDispatcher(IInterDispatcher* disp)
{
	dispatchThread->SetInterDispatcher(disp);
}

void SimpleIO::SetTimerDispatcher(ITimerDispatcher* disp)
{
	dispatchThread->SetTimerDispatcher(disp);
}

void SimpleIO::SetRedisDispatcher(IRedisDispatcher* disp)
{
	redisThread->SetRedisDispatcher(disp);
}

bool SimpleIO::Open(unsigned short port, IDispatcher* disp)
{
	return Open(nullptr, port, disp);
}

bool SimpleIO::Open(const char* ip, unsigned short port, IDispatcher* disp)
{
	Acceptor* acceptor = acceptorManager->CreateAcceptor(ioNotifier, sessionManager, disp);
	if (acceptor->Open(ip, port) == false)
		return false;

	return true;
}

bool SimpleIO::Connect(const char* ip, unsigned short port, IDispatcher* disp)
{
	Connector* connector = connectorManager->CreateConnector(ioNotifier, sessionManager, disp);
	if (connector->Connect(ip, port) == false)
		return false;

	return true;
}


