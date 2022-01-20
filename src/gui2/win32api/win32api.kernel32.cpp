module;
#include "../../assert/assert.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Win32Api : Kernel32;

export namespace shoujin::gui2::win32api {

HMODULE getModuleHandle(LPCTSTR lpModuleName)
{
	return SHOUJIN_ASSERT_WIN32(GetModuleHandle(lpModuleName));
}

void sleep(DWORD dwMilliseconds)
{
	Sleep(dwMilliseconds);
}

}
