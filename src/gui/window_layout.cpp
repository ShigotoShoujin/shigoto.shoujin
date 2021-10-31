#include "window_layout.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../assert/assert.hpp"

static constexpr int DEFAULT_WINDOW_SIZE_DIVIDER = 3;

static inline SIZE RectToSize(const RECT& rect);
static SIZE GetWindowSize(HWND hwnd);
static SIZE GetDefaultWindowSize(HWND hwnd);
static SIZE GetClientSizeFromWindowSize(const SIZE& window_size, DWORD style, DWORD exstyle);
static POINT GetCenteredPosition(const SIZE& window_size, HWND hparentwnd);

namespace shoujin::gui {

WindowLayout::WindowLayout(HWND hparentwnd)
{
	if(hparentwnd == nullptr) {
		_style = WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX;
		_exstyle = 0;
		hparentwnd = GetDesktopWindow();
	} else {
		_style = 0;
		_exstyle = WS_EX_CLIENTEDGE;
	}

	_window_size = GetDefaultWindowSize(hparentwnd);
	_client_size = GetClientSizeFromWindowSize(_window_size, _style, _exstyle);
	_position = GetCenteredPosition(_window_size, hparentwnd);
}

}

POINT _position;
SIZE _client_area, _window_area;
DWORD _style, _exstyle;

static inline SIZE RectToSize(const RECT& rect) { return {rect.right - rect.left, rect.bottom - rect.top}; }

static SIZE GetWindowSize(HWND hwnd)
{
	RECT parent_rect;
	SHOUJIN_ASSERT_WIN32(GetWindowRect(hwnd, &parent_rect));
	return RectToSize(parent_rect);
}

static SIZE GetDefaultWindowSize(HWND hwnd)
{
	SIZE size = GetWindowSize(hwnd);
	size.cx /= DEFAULT_WINDOW_SIZE_DIVIDER;
	size.cy /= DEFAULT_WINDOW_SIZE_DIVIDER;
	return size;
}

static SIZE GetClientSizeFromWindowSize(const SIZE& window_size, DWORD style, DWORD exstyle)
{
	RECT rect{0, 0, window_size.cx, window_size.cy};
	SHOUJIN_ASSERT_WIN32(AdjustWindowRectEx(&rect, style, 0, exstyle));
	SIZE adjusted_size = RectToSize(rect);
	return {
		window_size.cx - (adjusted_size.cx - window_size.cx),
		window_size.cy - (adjusted_size.cy - window_size.cy),
	};
}

static POINT GetCenteredPosition(const SIZE& window_size, HWND hparentwnd)
{
	RECT parent_rect;
	SHOUJIN_ASSERT_WIN32(GetWindowRect(hparentwnd, &parent_rect));
	SIZE parent_size = RectToSize(parent_rect);
	return {
		(parent_size.cx - window_size.cx) >> 1,
		(parent_size.cy - window_size.cy) >> 1,
	};
}
