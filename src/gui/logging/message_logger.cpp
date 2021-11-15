#include "../../file/file.hpp"
#include "../../file/logfile.hpp"
#include "message_string.hpp"

namespace shoujin::gui::logging {

thread_local bool _activate_wndproc_messagelog_;

void LogWndProcMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(!_activate_wndproc_messagelog_)
		return;

	if(msg == WM_SETCURSOR || msg == WM_NCHITTEST)
		return;

	auto debug_file = file::LogFile::GetDebugFile();
	auto file_exists = file::FileExists(debug_file);
	auto file_size = file_exists ? file::GetFileSize(debug_file) : 0;

	constexpr auto kMaxFileSize8mb = 0x800000;
	if(file_size > kMaxFileSize8mb) {
		file::FileDelete(debug_file);
		file_size = 0;
	}

	if(!file_size)
		file::LogFile::AppendDebug(FormatWindowMessageHeader());

	tstring msg_text = FormatWindowMessageLine(hwnd, msg, wparam, lparam);
	file::LogFile::AppendDebug(msg_text);
}

}
