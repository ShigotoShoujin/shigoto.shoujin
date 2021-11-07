#pragma once
#include "../types.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui::common_control {
	HWND _hwnd;
	HWND _hwnd_parent;

class ControlHandle {
public:
	ControlHandle(HWND hwnd_parent, LPCTSTR class_name, Point position, Size window_size)
	{

	}
};

}
