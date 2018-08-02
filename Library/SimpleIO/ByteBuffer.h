#pragma once

class ByteBuffer
{
private:
	char* buffer;		// ���ι���
	char* readptr;		// ���� ��ġ ������
	char* writeptr;		// �� ��ġ ������
	char* startptr;		// ������ ���� ������ (���Ǹ� ����)
	char* endptr;		// ������ �� ������ (���Ǹ� ����)
	int capacity;		// ������ �� ũ��

public:
	ByteBuffer(int size);
	~ByteBuffer();

	//void Initialize(char* buf, int len);
	int Capacity();
	int Length();
	int Remain();

	char* ReadPtr();
	char* WritePtr();
	char* StartPtr();
	char* EndPtr();

	void Read(int byte);
	void Write(int byte);
	void Arrange();

	void Copy(ByteBuffer* dest, int len);
	void Append(ByteBuffer* src);

	//static ByteBuffer* Create(int len);
	//static void Destroy(ByteBuffer* stream);
};

