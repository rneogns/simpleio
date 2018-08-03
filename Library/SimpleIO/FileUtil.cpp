#include "Precompiled.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include "FileUtil.h"

namespace fs = boost::filesystem;

bool FileUtil::IsFile(string path)
{
	return fs::is_regular_file(path);
}

bool FileUtil::IsDirectory(string path)
{
	return fs::is_directory(path);
}

bool FileUtil::Exists(string path)
{
	return fs::exists(path);
}

uintmax_t FileUtil::FileSize(string path)
{
	return fs::file_size(path);
}

bool FileUtil::CreateDirectory(string path)
{
	fs::path dir(path);

	if (fs::exists(dir))
		return false;

	return fs::create_directories(dir);
}

char FileUtil::GetSeparator()
{
	return fs::path::separator;
}
