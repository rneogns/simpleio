#include "Precompiled.h"
#include "MessageQueue.h"
#include "ByteBuffer.h"
#include "InterMsgHeader.h"
#include "Stream.h"

void MessageQueue::Push(QueueType type, Stream* s)
{
	std::lock_guard<std::mutex> guard(_lock[type]);

	streamQueue[type].push(s);
}

void MessageQueue::Push(QueueType type, InterMsgHeader* inter_msg)
{
	std::lock_guard<std::mutex> guard(_lock[type]);

	InterMsg* s = new InterMsg;
	s->PutBinary((char*)inter_msg, inter_msg->GetMsgSize());

	streamQueue[type].push(s);
}

Stream*  MessageQueue::Pop(QueueType type)
{
	std::lock_guard<std::mutex> guard(_lock[type]);

	if (streamQueue[type].empty() == true)
		return nullptr;

	Stream* s = streamQueue[type].front();
	streamQueue[type].pop();

	return s;	
}

size_t MessageQueue::Size(QueueType type)
{
	std::lock_guard<std::mutex> guard(_lock[type]);

	return streamQueue[type].size();
}

bool MessageQueue::Empty(QueueType type)
{
	std::lock_guard<std::mutex> guard(_lock[type]);

	return streamQueue[type].empty();
}