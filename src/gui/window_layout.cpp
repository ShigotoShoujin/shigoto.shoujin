#include "window_layout.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../assert/assert.hpp"
#include "types.hpp"

static constexpr int kDefaultWindowSizeDivider = 3;

using namespace shoujin::gui;

static inline Size RectToSize(const RECT& rect);
static Size GetWindowSize(HWND hwnd);
static Size GetDefaultWindowSize(HWND hwnd);
static Size GetClientSizeFromWindowSize(const Size& window_size, DWORD style, DWORD exstyle);
static Size GetWindowSizeFromClientSize(const Size& window_size, DWORD style, DWORD exstyle);
static Point GetCenteredPosition(const Size& window_size, HWND hparentwnd);
static void AdjustSizes(Size& window_size, Size& client_size, DWORD style, DWORD exstyle);

namespace shoujin::gui {

WindowLayout::WindowLayout(const CreateInfo& ci, HWND hparentwnd) :
	_position{ci.position},
	_window_size{ci.window_size},
	_client_size{ci.client_size},
	_style{ci.style ? ci.style : DefaultStyle},
	_exstyle{ci.exstyle}
{
	if(!hparentwnd)
		hparentwnd = GetDesktopWindow();

	if(!_window_size && !_client_size)
		_window_size = GetDefaultWindowSize(hparentwnd);

	AdjustSizes(_window_size, _client_size, _style, _exstyle);

	if(ci.create_mode == WindowLayout::CreateMode::Centered)
		_position = GetCenteredPosition(_window_size, hparentwnd);
}

}

static inline Size RectToSize(const RECT& rect) { return {rect.right - rect.left, rect.bottom - rect.top}; }

static Size GetWindowSize(HWND hwnd)
{
	RECT parent_rect;
	SHOUJIN_ASSERT_WIN32(GetWindowRect(hwnd, &parent_rect));
	return RectToSize(parent_rect);
}

static Size GetDefaultWindowSize(HWND hwnd)
{
	Size size = GetWindowSize(hwnd);
	size.x /= kDefaultWindowSizeDivider;
	size.y /= kDefaultWindowSizeDivider;
	return size;
}

static Size GetClientSizeFromWindowSize(const Size& window_size, DWORD style, DWORD exstyle)
{
	RECT rect{0, 0, window_size.x, window_size.y};
	SHOUJIN_ASSERT_WIN32(AdjustWindowRectEx(&rect, style, 0, exstyle));
	Size adjusted_size = RectToSize(rect);
	return {
		window_size.x - (adjusted_size.x - window_size.x),
		window_size.y - (adjusted_size.y - window_size.y),
	};
}

static Size GetWindowSizeFromClientSize(const Size& client_size, DWORD style, DWORD exstyle)
{
	RECT rect{0, 0, client_size.x, client_size.y};
	SHOUJIN_ASSERT_WIN32(AdjustWindowRectEx(&rect, style, 0, exstyle));
	Size adjusted_size = RectToSize(rect);
	return {adjusted_size.x, adjusted_size.y};
}

static Point GetCenteredPosition(const Size& window_size, HWND hparentwnd)
{
	RECT parent_rect;
	SHOUJIN_ASSERT_WIN32(GetWindowRect(hparentwnd, &parent_rect));
	Size parent_size = RectToSize(parent_rect);
	return {
		(parent_size.x - window_size.x) >> 1,
		(parent_size.y - window_size.y) >> 1,
	};
}

/// <summary>
/// When window_size is specified, adjust client_size.
/// Else when client_size is specified, adjust window_size.
/// ASSERT if none are specified
/// </summary>
/// <param name="client_size">The size of the client area. Only used when window_size is not specified</param>
/// <param name="window_size">The size of the window. Has priority over client_size</param>
static void AdjustSizes(Size& window_size, Size& client_size, DWORD style, DWORD exstyle)
{
	if(!window_size)
		window_size = GetWindowSizeFromClientSize(client_size, style, exstyle);
	else
		client_size = GetClientSizeFromWindowSize(window_size, style, exstyle);
}
