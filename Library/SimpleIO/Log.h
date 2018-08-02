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
	wstring prefix;			// �α� ���λ� ��) "Debug" ���� -> Debug20161017.log	
	bool isWriteTime;		// �ð��� ������ ����
	bool isWriteFuncName;	// �Լ��̸��� ���ι�ȣ�� ������ ����
	bool isWritePerHour;	// �ѽð����� �α׸� ���� ������ ���� (false�̸� �⺻ �Ϸ����, true�̸� Debug20161017-06.log ������ -�ð��� ����)

public:
	LogFile(LogType type, wstring prefix, bool writeTime, bool writeFuncName, bool perHour);
	~LogFile();

	void OpenFile(const wstring& logPath);
	void Write(const wstring& logPath, const wchar_t* funcName, int lineNo, const wchar_t* message);	
};

class Log
{
	vector<LogFile> logFiles;
	wstring path;			// �α׸� ������ ���丮

public:
	Log();
	static Log& GetInstance();

	void WriteToFile(LogType type, const wchar_t* funcName, int lineNo, const wchar_t* message);
	void SetLogPath(wstring dir);
};

class LogHelper
{
private:
	LogType logType;
	const wchar_t* funcName;
	int lineNo;

public:
	LogHelper(LogType type, const wchar_t* funcName, int lineNo);

	void operator()(const wchar_t* message, ...);
};

#define LOG_DEBUG	LogHelper(LogType::DEBUG, __FUNCTIONW__, __LINE__)
#define LOG_CORE	LogHelper(LogType::CORE, __FUNCTIONW__, __LINE__)
#define LOG_INFO	LogHelper(LogType::INFO, __FUNCTIONW__, __LINE__)
#define LOG_CRI		LogHelper(LogType::CRI, __FUNCTIONW__, __LINE__)