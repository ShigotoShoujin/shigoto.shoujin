module;
#include "../../assert/assert.hpp"

export module Shoujin.Gui.Win32Api : User32;

export namespace shoujin::gui2::win32api {

BOOL UpdateWindow(HWND hWnd)
{
	return SHOUJIN_ASSERT(::UpdateWindow(hWnd));
}

}
