#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sstream>

#ifndef _DEBUG

namespace Debug
{
	void DrawDebugTabOrder(HDC hdc, RECT control_rect, int taborder){};
}

#else

namespace Debug
{
	void DrawDebugTabOrder(HDC hdc, const RECT& control_rect, int taborder)
	{
		std::wstringstream ss;
		ss << "TabOrder=" << taborder;

		const auto& text = ss.str();
		RECT rect = control_rect;
		DrawText(hdc, text.c_str(), (int)text.length(), &rect, 0);
	}
}

#endif
