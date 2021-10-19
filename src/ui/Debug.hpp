#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Debug
{
	void DrawDebugTabOrder(HDC hdc, const RECT& control_rect, int taborder);
}
