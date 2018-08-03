#pragma once

#include "MemoryPool.h"
#include "PacketHeader.h"
#include "InterMsgHeader.h"

class ByteBuffer;
class Session;

class Stream
{
protected:
	ByteBuffer* byteBuffer;
	Session* session;

public:
	Stream();
	Stream(ByteBuffer* buffer);
	
	void Clear();

	ByteBuffer* GetByteBuffer();
	char* GetReadBuffer();
	char* GetWriteBuffer();
	
	Session* GetSession();
	void SetSession(Session* s);

	bool GetByte(char& o);
	bool GetByte(unsigned char& o);
	bool GetShort(short& o);
	bool GetShort(unsigned short& o);
	bool GetInt(int& o);
	bool GetInt(unsigned int& o);
	bool GetInt64(long long& o);
	bool GetInt64(unsigned long long& o);
	bool GetFloat(float& o);
	bool GetDouble(double& o);
	bool GetBinary(char* out, int size);
	int GetString(char* out);

	bool PutByte(char v);
	bool PutByte(unsigned char v);
	bool PutShort(short v);
	bool PutShort(unsigned short v);
	bool PutInt(int v);
	bool PutInt(unsigned int v);
	bool PutInt64(long long v);
	bool PutInt64(unsigned long long v);
	bool PutFloat(float v);
	bool PutDouble(double v);
	bool PutBinary(char* v, int size);
	bool PutString(char* v, int size);

	template <typename T>
	bool Get(T& out)
	{
		out = (*(T *)byteBuffer->ReadPtr());
		byteBuffer->Read((int)sizeof(T));
	}

	template <typename T>
	bool Put(T value)
	{
		*byteBuffer->WritePtr() = value;
		byteBuffer->Write((int)sizeof(T));
	}
};

class Packet 
	: public Stream
	, public MemoryPool<Packet, PACKET_POOL_INIT_COUNT>
{
public:
	Packet();
	~Packet();

	PacketHeader* GetHeader(); // ����� �о�� �뵵, ����(��������)�� ��Ŷ�� ������ ��Ŷ����� �ѱ�� �뵵�� ���, ������ ���� ��ġ �̵�
	PacketHeader* GetHeaderPtr();  // ������ �����͸� ����� ��ȯ�Ͽ� ��ȯ, ������ ���� ��ġ�� �ű��� ����

	void MakePacket(unsigned short packetNo);
	void EndPacket();
};

class InterMsg 
	: public Stream
	, public MemoryPool<InterMsg, INTERMSG_POOL_INIT_COUNT>
{
public:
	InterMsg();
	~InterMsg();

	InterMsgHeader* GetHeaderPtr();  // ������ �����͸� ����� ��ȯ�Ͽ� ��ȯ, ������ ���� ��ġ�� �ű��� ����

	void MakeMessage(unsigned short packetNo);
	void EndMessage();
};