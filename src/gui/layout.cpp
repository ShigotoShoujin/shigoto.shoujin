#include "../assert/assert.hpp"
#include "layout.hpp"

static constexpr int kDefaultWindowSizeDivider = 3;

using namespace shoujin::gui;

static Size GetWindowSize(HWND hwnd);
static Size GetDefaultWindowSize(HWND hwnd);
static Size GetClientSizeFromWindowSize(Size const& window_size, DWORD style, DWORD exstyle);
static Size GetWindowSizeFromClientSize(Size const& client_size, DWORD style, DWORD exstyle);
static Point GetCenteredPosition(Size const& window_size, HWND hparentwnd);
static void AdjustSizes(Size& window_size, Size& client_size, DWORD style, DWORD exstyle);

namespace shoujin::gui {

Layout::Layout(LayoutParam const& layout_param) :
	_position{layout_param.position},
	_window_size{layout_param.window_size},
	_client_size{layout_param.client_size},
	_style{layout_param.style},
	_exstyle{layout_param.exstyle},
	_anchor{layout_param.anchor},
	_tabstop{layout_param.tabstop},
	_text{layout_param.text}
{
	HWND hparentwnd = GetDesktopWindow();

	if(!_window_size && !_client_size)
		_window_size = GetDefaultWindowSize(hparentwnd);

	AdjustSizes(_window_size, _client_size, _style, _exstyle);

	if(layout_param.layout_mode == LayoutMode::CenterParent)
		_position = GetCenteredPosition(_window_size, hparentwnd);
}

void Layout::SetTabStop(bool tabstop)
{
	_tabstop = tabstop;
}

void Layout::SetStyle(DWORD style, DWORD exstyle)
{
	if(_style == style && _exstyle == exstyle)
		return;

	_style = style;
	_exstyle = exstyle;
	AdjustSizes(_window_size, _client_size, _style, _exstyle);
}

void Layout::SetWindowSize(Size const& window_size)
{
	_window_size = window_size;
	_client_size = GetClientSizeFromWindowSize(window_size, _style, _exstyle);
}

void Layout::SetRectFromHandle(HWND hwnd)
{
	RECT rect;

	SHOUJIN_ASSERT_WIN32(::GetWindowRect(hwnd, &rect));
	_position.x = rect.left;
	_position.y = rect.right;
	_window_size = RectToSize(rect);

	SHOUJIN_ASSERT_WIN32(::GetClientRect(hwnd, &rect));
	_client_size = RectToSize(rect);
}

void Layout::SetLayoutFromHandle(HWND hwnd)
{
	SetRectFromHandle(hwnd);

	auto gwlp = [&hwnd](int index) {
		auto longptr = GetWindowLongPtr(hwnd, index);
		return *reinterpret_cast<DWORD*>(&longptr);
	};

	_style = gwlp(GWL_STYLE);
	_exstyle = gwlp(GWL_EXSTYLE);
}

void Layout::SetLayout(Layout layout)
{
	OnLayoutResetEvent(layout);

	_position = layout._position;
	_window_size = layout._window_size;
	_client_size = layout._client_size;
	_style = layout._style;
	_exstyle = layout._exstyle;
	_anchor = layout._anchor;
	_tabstop = layout._tabstop;
	_text = layout._text;
}

}

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

static Size GetClientSizeFromWindowSize(Size const& window_size, DWORD style, DWORD exstyle)
{
	RECT rect{0, 0, window_size.x, window_size.y};
	SHOUJIN_ASSERT_WIN32(AdjustWindowRectEx(&rect, style, 0, exstyle));
	Size adjusted_size = RectToSize(rect);
	return {
		window_size.x - (adjusted_size.x - window_size.x),
		window_size.y - (adjusted_size.y - window_size.y),
	};
}

static Size GetWindowSizeFromClientSize(Size const& client_size, DWORD style, DWORD exstyle)
{
	RECT rect{0, 0, client_size.x, client_size.y};
	SHOUJIN_ASSERT_WIN32(AdjustWindowRectEx(&rect, style, 0, exstyle));
	Size adjusted_size = RectToSize(rect);
	return {adjusted_size.x, adjusted_size.y};
}

static Point GetCenteredPosition(Size const& window_size, HWND hparentwnd)
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
