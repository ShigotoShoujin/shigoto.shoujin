#include "../../file/file.hpp"
#include "../../file/logfile.hpp"
#include "message_string.hpp"

namespace shoujin::gui::logging {

thread_local bool _activate_wndproc_messagelog_;

void LogMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(!_activate_wndproc_messagelog_)
		return;

	auto debug_file = file::LogFile::GetDebugFile();
	if(!file::FileExists(debug_file) || file::GetFileSize(debug_file) == 0)
		file::LogFile::AppendDebug(FormatWindowMessageHeader());

	tstring msg_text = FormatWindowMessageLine(hwnd, msg, wparam, lparam);
	file::LogFile::AppendDebug(msg_text);
}

}
