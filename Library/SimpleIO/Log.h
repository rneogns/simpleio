#pragma once

enum LogType
{
	DEBUG,		// ������ �α�
	CORE,		// �ý��� �ھ�(Ȥ�� ���̺귯����)�� �α�
	INFO,		// �⺻ �α�
	CRI,		// �߻��ϸ� �ȵǴ� �ɰ��� �α�
	LOG_TYPE_MAX,
};

class LogFile
{
	FILE* fp;
	LogType logType;		// enum LogType
	string prefix;			// �α� ���λ� ��) "Debug" ���� -> Debug20161017.log	
	bool isWriteTime;		// �ð��� ������ ����
	bool isWriteFuncName;	// �Լ��̸��� ���ι�ȣ�� ������ ����
	bool isWritePerHour;	// �ѽð����� �α׸� ���� ������ ���� (false�̸� �⺻ �Ϸ����, true�̸� Debug20161017-06.log ������ -�ð��� ����)

public:
	LogFile(LogType type, string prefix, bool writeTime, bool writeFuncName, bool perHour);
	~LogFile();

	void OpenFile(const string& logPath);
	void Write(const string& logPath, const char* funcName, int lineNo, const char* message);	
};

class Log
{
	vector<LogFile> logFiles;
	string path;			// �α׸� ������ ���丮

public:
	Log();
	static Log& GetInstance();

	void WriteToFile(LogType type, const char* funcName, int lineNo, const char* message);
	void SetLogPath(string dir);
};

class LogHelper
{
private:
	LogType logType;
	const char* funcName;
	int lineNo;

public:
	LogHelper(LogType type, const char* funcName, int lineNo);

	void operator()(const char* message, ...);
};

#define LOG_DEBUG	LogHelper(LogType::DEBUG, __FUNCTION__, __LINE__)
#define LOG_CORE	LogHelper(LogType::CORE, __FUNCTION__, __LINE__)
#define LOG_INFO	LogHelper(LogType::INFO, __FUNCTION__, __LINE__)
#define LOG_CRI		LogHelper(LogType::CRI, __FUNCTION__, __LINE__)