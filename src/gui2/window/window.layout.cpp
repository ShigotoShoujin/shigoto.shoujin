module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Window : Layout;
import : Size;
import Shoujin.Gui.Win32Api;

using namespace shoujin::gui2;

export namespace shoujin::gui2::layout {

Size getScreenSize()
{
	return {
		win32api::getSystemMetrics(SM_CXSCREEN),
		win32api::getSystemMetrics(SM_CYSCREEN)};
}

}
