#include "../assert/assert.hpp"
#include "../tstring.hpp"
#include <filesystem>
#include <string>

namespace shoujin::file {

[[nodiscard]] tstring GetProcessFile()
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

[[nodiscard]] bool DirectoryExists(tstring path)
{
	std::error_code ec;
	return std::filesystem::is_directory(path, ec);
}

[[nodiscard]] bool FileExists(tstring file)
{
	std::error_code ec;
	return std::filesystem::is_regular_file(file, ec);
}

[[nodiscard]] uintmax_t GetFileSize(tstring file)
{
	return std::filesystem::file_size(file);
}

[[nodiscard]] bool TryFileDelete(tstring file)
{
	std::error_code remove_error_code;
	return std::filesystem::remove(file, remove_error_code);
}

}
