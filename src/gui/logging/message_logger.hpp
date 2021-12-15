#ifndef SHOUJIN_SOURCE_GUI_LOGGING_MESSAGE_LOGGER
#define SHOUJIN_SOURCE_GUI_LOGGING_MESSAGE_LOGGER

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui::logging {

extern thread_local bool _activate_wndproc_messagelog_;

void LogWndProcMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

}

#endif
