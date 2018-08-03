#pragma once

class FileUtil
{
public:
	static bool IsFile(string path);
	static bool IsDirectory(string path);
	static bool Exists(string path);
	static uintmax_t FileSize(string path);
	static bool CreateDirectory(string path);	
	static char GetSeparator();
};