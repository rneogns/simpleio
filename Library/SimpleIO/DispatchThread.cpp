#include "Precompiled.h"
#include "DispatchThread.h"
#include "SimpleIO.h"
#include "Session.h"
#include "SessionManager.h"
#include "IONotifier.h"
#include "MessageQueue.h"
#include "Timer.h"
#include "TimerQueue.h"
#include "Stream.h"
#include "ByteBuffer.h"
#include "Dispatcher.h"

DispatchThread::DispatchThread(SimpleIO* simpleio)
	: _simpleio(simpleio)
	, _interDispatcher(nullptr)
	, _timerDispatcher(nullptr)
{
}


void DispatchThread::SetInterDispatcher(IInterDispatcher* disp)
{
	_interDispatcher = disp;
}


void DispatchThread::SetTimerDispatcher(ITimerDispatcher* disp)
{
	_timerDispatcher = disp;
}

void* DispatchThread::Process()
{
	while (_simpleio->isRunning.load())
	{
		MessageQueue* msgQueue = _simpleio->GetMessageQueue();
		TimerQueue* timerQueue = _simpleio->GetTimerQueue();

		if (msgQueue->Empty(QueueType::PACKET_Q) 
			&& msgQueue->Empty(QueueType::INTER_MSG_Q)
			&& timerQueue->Empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		
		// 타이머 메시지 처리
		{
			Timer timer;
			if (timerQueue->Pop(timer))
			{
				// 지정된 시간이 지난 타이머가 있는가?

				// 타이머를 처리한다
				if (_timerDispatcher)
					_timerDispatcher->Dispatch(timer);

				// 반복이 설정되어 있을 경우 타이머를 다시 넣어준다
				if (timer.repeat)
					timerQueue->Push(timer);
			}
		}

		// 인터메시지 처리
		{
			Stream* stream = msgQueue->Pop(QueueType::INTER_MSG_Q);
			if (stream)
			{
				// 인터디스패처가 설정되어 있지 않은 경우가 있어서 null검사를 한다.
				// 넘겨주는 session이 null인 경우도 있다.(타겟이 없거나 나간 경우)
				if (_interDispatcher)
					_interDispatcher->Dispatch((InterMsg*)stream);
			}
		}

		// 클라이언트 패킷처리
		{
			Stream* stream = msgQueue->Pop(QueueType::PACKET_Q);
			if (stream)
			{
				Session* session = stream->GetSession();
				
				if (_simpleio->GetSessionManager()->IsValid(session))
					session->Dispatch((Packet*)stream);
			}
		}
	}

	return nullptr;
}
