#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../../tstring.hpp"

namespace shoujin::gui::logging {

LPCTSTR LookupWindowMessageText(UINT msg);
tstring FormatWindowMessageText(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
tstring FormatWindowMessageHeader();
tstring FormatWindowMessageLine(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

}
