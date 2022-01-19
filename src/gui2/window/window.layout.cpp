module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Window : Layout;
import Shoujin.Gui.Types;
import Shoujin.Gui.Win32Api;

export namespace shoujin::gui2::layout {

Size getScreenSize()
{
	return {
		win32api::getSystemMetrics(SM_CXSCREEN),
		win32api::getSystemMetrics(SM_CYSCREEN)};
}

}
