module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Window : Layout;
import : Point;
import : Rectangle;
import : Size;
import : Style;
import Shoujin.Win32Api;

using namespace shoujin::gui2;

namespace shoujin::gui2::layout {

export
{
	Point getCenteredPosition(Size const& windowSize, HWND hParentWnd = {});
	Point getCenteredPosition(Size const& windowSize, Size const& parentSize);
	Size getClientSizeFromWindowSize(Size const& windowSize, DWORD style, DWORD exstyle);
	Size getDefaultClientSize(HWND hParentWnd = {});
	WindowStyle getDefaultStyle();
	Size getParentSize(HWND hParentWnd = {});
	Size getWindowSizeFromClientSize(Size const& clientSize, DWORD style, DWORD exstyle);
}

Point getCenteredPosition(Size const& windowSize, HWND hParentWnd)
{
	return getCenteredPosition(windowSize, getParentSize(hParentWnd));
}

Point getCenteredPosition(Size const& windowSize, Size const& parentSize)
{
	return {
		(parentSize.x - windowSize.x) >> 1,
		(parentSize.y - windowSize.y) >> 1,
	};
}

Size getClientSizeFromWindowSize(Size const& windowSize, DWORD style, DWORD exstyle)
{
	RECT rect{0, 0, windowSize.x, windowSize.y};
	win32api::adjustWindowRectEx(&rect, style, 0, exstyle);
	Size adjustedSize = RectangleToSize(rect);
	return {
		windowSize.x - (adjustedSize.x - windowSize.x),
		windowSize.y - (adjustedSize.y - windowSize.y),
	};
}

Size getDefaultClientSize(HWND hParentWnd)
{
	return getParentSize(hParentWnd) / 4;
}

WindowStyle getDefaultStyle()
{
	// clang-format off
	return
		WindowStyle::Border |
		WindowStyle::Caption |
		WindowStyle::MaximizeBox |
		WindowStyle::MinimizeBox |
		WindowStyle::Sizable |
		WindowStyle::SystemMenu;
	// clang-format on
}

Size getParentSize(HWND hParentWnd)
{
	if(hParentWnd == NULL)
		hParentWnd = win32api::getDesktopWindow();

	RECT parentRect;
	win32api::getWindowRect(hParentWnd, &parentRect);

	return RectangleToSize(parentRect);
}

Size getWindowSizeFromClientSize(Size const& clientSize, DWORD style, DWORD exstyle)
{
	RECT rect{0, 0, clientSize.x, clientSize.y};
	win32api::adjustWindowRectEx(&rect, style, 0, exstyle);
	Size adjustedSize = RectangleToSize(rect);
	return {adjustedSize.x, adjustedSize.y};
}

}
