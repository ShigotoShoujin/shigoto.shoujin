#ifndef SHOUJIN_SOURCE_GUI_LOGGING_MESSAGE_STRING
#define SHOUJIN_SOURCE_GUI_LOGGING_MESSAGE_STRING

import Shoujin.String;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
namespace shoujin::gui::logging {

LPCTSTR LookupWindowMessageText(UINT msg);
tstring FormatWindowMessageText(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
tstring FormatWindowMessageHeader();
tstring FormatWindowMessageLine(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

}

#endif
