#include "Precompiled.h"
#include "ByteBuffer.h"

ByteBuffer::ByteBuffer(int size)
	: buffer(new char[size])
	, readptr(buffer)
	, writeptr(buffer)
	, startptr(buffer)
	, endptr(buffer + size)
	, capacity(size)
{
}

ByteBuffer::~ByteBuffer()
{
	delete buffer;
}
/*
void ByteBuffer::Initialize(char* buf, int len)
{
	buffer = buf;
	writeptr = buffer;
	readptr = buffer;
	startptr = buffer;
	endptr = buffer + len;
	capacity = len;
}
*/
int ByteBuffer::Capacity()
{
	return capacity;
}

int ByteBuffer::Length()
{
	return (int)(writeptr - readptr);
}

int ByteBuffer::Remain()
{
	return (int)(endptr - writeptr);
}

char* ByteBuffer::ReadPtr()
{
	return readptr;
}

char* ByteBuffer::WritePtr()
{
	return writeptr;
}

char* ByteBuffer::StartPtr()
{
	return startptr;
}

char* ByteBuffer::EndPtr()
{
	return endptr;
}

void ByteBuffer::Read(int byte)
{
	readptr += byte;

	if (readptr > writeptr)
	{
		assert(false);
	}
}

void ByteBuffer::Write(int byte)
{
	writeptr += byte;

	if (writeptr > endptr)
	{
		assert(false);
	}
}

void ByteBuffer::Arrange()
{
	int len = Length();
	if (len == 0)
	{
		readptr = startptr;
		writeptr = startptr;
	}
	else
	{
		memmove(startptr, readptr, len);
		readptr = startptr;
		writeptr = startptr + len;
	}
}

void ByteBuffer::Copy(ByteBuffer* dest, int len)
{
	if (len > Length())
		return;

	memcpy(dest->WritePtr(), this->ReadPtr(), len);
	
	dest->Write(len);

	this->Read(len);
}

void ByteBuffer::Append(ByteBuffer* src)
{
	if (src->Length() == 0)
	{
		return;
	}

	// 복사할 크기가 현재 버퍼의 남은크기보다 큰 경우
	if (src->Length() > Remain())
	{
		return;
	}

	// 데이터를 복사하고
	memcpy(WritePtr(), src->ReadPtr(), src->Length());

	// 쓰기위치를 옮겨준다
	Write(src->Length());

	// 복사한 데이터도 복사된 만큼 읽기위치를 옮겨준다
	src->Read(src->Length());
}
/*
ByteBuffer* ByteBuffer::Create(int len)
{
	char* buf = new char[len];
	ByteBuffer* stream = new ByteBuffer;

	stream->Initialize(buf, len);

	return stream;
}

void ByteBuffer::Destroy(ByteBuffer* stream)
{
	if (stream == nullptr)
		return;
	
	char* buffer = stream->StartPtr();

	SAFE_DELETE(buffer);

	SAFE_DELETE(stream);
}
*/
