#include "Precompiled.h"
#include "BaseDefines.h"
#include "Log.h"
#include "TimeUtil.h"
#include "FileUtil.h"

#define LINE_BUFFER 32768

LogFile::LogFile(LogType type, string prefix, bool writeTime, bool writeFuncName, bool writePerHour)
	: fp(0)
	, logType(type)
	, prefix(prefix)
	, isWriteTime(writeTime)
	, isWriteFuncName(writeFuncName)
	, isWritePerHour(writePerHour)
{
}

LogFile::~LogFile()
{
	if (fp != NULL)
		fclose(fp);
}

void LogFile::OpenFile(const string& logPath)
{
	char filename[MAX_FILENAME_LEN] = { 0, };

	struct tm nowTM;
	TimeUtil::CurTimeTM(&nowTM);

	if (isWritePerHour)
		sprintf(filename, "%s%04d%02d%02d-%02d.log", prefix.c_str(), nowTM.tm_year + 1900, nowTM.tm_mon + 1, nowTM.tm_mday, nowTM.tm_hour);
	else
		sprintf(filename, "%s%04d%02d%02d.log", prefix.c_str(), nowTM.tm_year + 1900, nowTM.tm_mon + 1, nowTM.tm_mday);

	// 로그 디렉토리로 지정된 디렉토리가 없다면 생성해준다
	if (FileUtil::Exists(logPath) == false)
		FileUtil::CreateDirectory(logPath);

	string fullpath(logPath);
	if (fullpath.back() == '/')
		fullpath += filename;
	else
		fullpath = fullpath + '/' + filename;

	fp = fopen(fullpath.c_str(), "ab");
}

void LogFile::Write(const string& logPath, const char* funcName, int lineNo, const char* message)
{
	static char header[128] = { 0, };
	static char buffer[LINE_BUFFER] = { 0, };

	if (logPath.empty())
		return;

	if (fp == NULL)
		OpenFile(logPath);

	if (isWriteTime)
	{
		struct tm nowTM;
		TimeUtil::CurTimeTM(&nowTM);
		sprintf(header, "[%02d:%02d:%02d]", nowTM.tm_hour, nowTM.tm_min, nowTM.tm_sec);
	}

	if (isWriteFuncName)
		wsprintf(header, "%s %s(%d)", header, funcName, lineNo);

	fprintf(fp, "%s: %s\r\n", header, message);
	printf("%s: %s\r\n", header, message);

	fflush(fp);
}

Log::Log()
{
	logFiles.push_back(LogFile(LogType::DEBUG	, "Debug"	, true, true, false));
	logFiles.push_back(LogFile(LogType::CORE	, "Core"	, true, true, false));
	logFiles.push_back(LogFile(LogType::INFO	, "Info"	, true, true, false));
	logFiles.push_back(LogFile(LogType::CRI		, "Cri"	, true, true, false));
}

Log& Log::GetInstance()
{
	static Log inst;
	return inst;
}

void Log::WriteToFile(LogType type, const char* funcName, int lineNo, const char* message)
{
	logFiles[type].Write(path, funcName, lineNo, message);
}

void Log::SetLogPath(string dir)
{
	path = dir;
}

LogHelper::LogHelper(LogType type, const char* funcName, int lineNo)
	: logType(type)
	, funcName(funcName)
	, lineNo(lineNo)
{
}

void LogHelper::operator()(const char* message, ...)
{
	static char buffer[LINE_BUFFER] = { 0, };

	va_list args;
	va_start(args, message);
	wvsprintf(buffer, message, args);
	va_end(args);

	Log::GetInstance().WriteToFile(logType, funcName, lineNo, buffer);
}