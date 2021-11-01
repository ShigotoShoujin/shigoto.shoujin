#include "window_layout.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "operators.hpp"
#include "../assert/assert.hpp"

static constexpr int DEFAULT_WINDOW_SIZE_DIVIDER = 3;
static constexpr DWORD DEFAULT_STYLE = WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX;

static inline SIZE RectToSize(const RECT& rect);
static SIZE GetWindowSize(HWND hwnd);
static SIZE GetDefaultWindowSize(HWND hwnd);
static SIZE GetClientSizeFromWindowSize(const SIZE& window_size, DWORD style, DWORD exstyle);
static SIZE GetWindowSizeFromClientSize(const SIZE& window_size, DWORD style, DWORD exstyle);
static POINT GetCenteredPosition(const SIZE& window_size, HWND hparentwnd);
static void AdjustSizes(SIZE& window_size, SIZE& client_size, DWORD style, DWORD exstyle);

namespace shoujin::gui {

WindowLayout::WindowLayout()
{
	HWND hparentwnd = GetDesktopWindow();

	_style = DEFAULT_STYLE;
	_exstyle = 0;

	_window_size = GetDefaultWindowSize(hparentwnd);
	_client_size = GetClientSizeFromWindowSize(_window_size, _style, _exstyle);
	_position = GetCenteredPosition(_window_size, hparentwnd);
}

WindowLayout::WindowLayout(const CreateInfo& ci) :
	_position{ci.position},
	_window_size {ci.window_size},
	_client_size {ci.client_size},
	_style{ci.style ? ci.style : DEFAULT_STYLE},
	_exstyle{ci.exstyle}
{
	AdjustSizes(_window_size, _client_size, _style, _exstyle);
}

}

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

static SIZE GetWindowSizeFromClientSize(const SIZE& client_size, DWORD style, DWORD exstyle)
{
	RECT rect{0, 0, client_size.cx, client_size.cy};
	SHOUJIN_ASSERT_WIN32(AdjustWindowRectEx(&rect, style, 0, exstyle));
	SIZE adjusted_size = RectToSize(rect);
	return {adjusted_size.cx, adjusted_size.cy};
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

/// <summary>
/// When window_size is specified, adjust client_size.
/// Else when client_size is specified, adjust window_size.
/// ASSERT if none are specified
/// </summary>
/// <param name="client_size">The size of the client area. Only used when window_size is not specified</param>
/// <param name="window_size">The size of the window. Has priority over client_size</param>
static void AdjustSizes(SIZE& window_size, SIZE& client_size, DWORD style, DWORD exstyle)
{
	SHOUJIN_ASSERT(window_size != 0 || client_size != 0);

	if(window_size == 0)
		window_size = GetWindowSizeFromClientSize(client_size, style, exstyle);
	else
		client_size = GetClientSizeFromWindowSize(window_size, style, exstyle);
}
