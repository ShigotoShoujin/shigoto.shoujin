#include "logfile.hpp"
#include <filesystem>
#include <fstream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static void AppendToFile(std::filesystem::path file, shoujin::tstring text, bool newline = false)
{
	std::basic_ofstream<TCHAR> ofs(file, std::ios::binary | std::ios::app);
	ofs.write(text.c_str(), text.length());
	if(newline)
		ofs << std::endl;
}

namespace shoujin::file {

#ifndef NDEBUG

#ifndef SHOUJIN_DEBUG_LOGFILE
#define SHOUJIN_DEBUG_LOGFILE TEXT("shoujin.log")
#endif

tstring LogFile::GetDebugFile()
{
	static std::filesystem::path file;

	if(file.empty()) {
		auto path = std::filesystem::temp_directory_path();
		file = path.replace_filename(SHOUJIN_DEBUG_LOGFILE);
	}

	return string::ToTString(file);
}

void LogFile::AppendDebug(tstring text)
{
	AppendToFile(GetDebugFile(), text);
}

void LogFile::AppendLineDebug(tstring text)
{
	AppendToFile(GetDebugFile(), text, true);
}

#endif

void LogFile::Append(tstring_view file, tstring text)
{
	AppendToFile(file, text);
}

void LogFile::AppendLine(tstring_view file, tstring text)
{
	AppendToFile(file, text, true);
}

}
