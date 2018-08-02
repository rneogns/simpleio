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
		
		// Ÿ�̸� �޽��� ó��
		{
			Timer timer;
			if (timerQueue->Pop(timer))
			{
				// ������ �ð��� ���� Ÿ�̸Ӱ� �ִ°�?

				// Ÿ�̸Ӹ� ó���Ѵ�
				if (_timerDispatcher)
					_timerDispatcher->Dispatch(timer);

				// �ݺ��� �����Ǿ� ���� ��� Ÿ�̸Ӹ� �ٽ� �־��ش�
				if (timer.repeat)
					timerQueue->Push(timer);
			}
		}

		// ���͸޽��� ó��
		{
			Stream* stream = msgQueue->Pop(QueueType::INTER_MSG_Q);
			if (stream)
			{
				// ���͵���ó�� �����Ǿ� ���� ���� ��찡 �־ null�˻縦 �Ѵ�.
				// �Ѱ��ִ� session�� null�� ��쵵 �ִ�.(Ÿ���� ���ų� ���� ���)
				if (_interDispatcher)
					_interDispatcher->Dispatch((InterMsg*)stream);
			}
		}

		// Ŭ���̾�Ʈ ��Ŷó��
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
