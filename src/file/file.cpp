#include <filesystem>
#include <string>
#include "../tstring.hpp"
#include "../assert/assert.hpp"

namespace shoujin::file {

tstring GetProcessFile()
{
	TCHAR out_file[MAX_PATH]{};
	SHOUJIN_ASSERT_WIN32(GetModuleFileName(GetModuleHandle(nullptr), out_file, MAX_PATH));
	return out_file;
}

void FileDelete(tstring file)
{
	std::error_code remove_error_code;
	std::filesystem::remove(file, remove_error_code);
	SHOUJIN_ASSERT_STDERRORCODE(remove_error_code);
}

bool DirectoryExists(tstring path)
{
	std::error_code ec;
	return std::filesystem::is_directory(path, ec);
}

bool FileExists(tstring file)
{
	std::error_code ec;
	return std::filesystem::is_regular_file(file, ec);
}

bool TryFileDelete(tstring file)
{
	std::error_code remove_error_code;
	return std::filesystem::remove(file, remove_error_code);
}

}
