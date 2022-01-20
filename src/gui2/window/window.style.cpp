module;
#include "../../enum_operators.hpp"
#include <utility>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Window : Style;
import Shoujin.Geometry;

export namespace shoujin::gui2 {

// clang-format off
enum class WindowStyle : int {
	None             = 0x0000,
	Border           = 0x0001,
	BorderClientEdge = 0x0002,
	BorderStaticEdge = 0x0004,
	BorderWindowEdge = 0x0008,
	Caption          = 0x0010,
	Disabled         = 0x0020,
	MaximizeBox      = 0x0040,
	MinimizeBox      = 0x0080,
	Sizable          = 0x0100,
	SystemMenu       = 0x0200,
	TopMost          = 0x0400
};
// clang-format on

SHOUJIN_DEFINE_ENUM_FLAG_OPERATORS(WindowStyle);

constexpr std::pair<DWORD, DWORD> styleToNative(WindowStyle windowStyle)
{
	auto conv = [windowStyle](DWORD& out, WindowStyle from, DWORD to) {
		if((windowStyle & from) != WindowStyle::None)
			out |= to;
	};

	DWORD style = 0;
	DWORD exstyle = 0;

	conv(style, WindowStyle::Border, WS_BORDER);
	conv(style, WindowStyle::Caption, WS_DLGFRAME);
	conv(style, WindowStyle::Disabled, WS_DISABLED);
	conv(style, WindowStyle::MaximizeBox, WS_MAXIMIZEBOX);
	conv(style, WindowStyle::MinimizeBox, WS_MINIMIZEBOX);
	conv(style, WindowStyle::Sizable, WS_SIZEBOX);
	conv(style, WindowStyle::SystemMenu, WS_SYSMENU);
	conv(exstyle, WindowStyle::BorderClientEdge, WS_EX_CLIENTEDGE);
	conv(exstyle, WindowStyle::BorderStaticEdge, WS_EX_STATICEDGE);
	conv(exstyle, WindowStyle::BorderWindowEdge, WS_EX_WINDOWEDGE);
	conv(exstyle, WindowStyle::TopMost, WS_EX_TOPMOST);

	return {style, exstyle};
}

constexpr WindowStyle styleFromNative(DWORD style, DWORD exstyle)
{
	WindowStyle ws = WindowStyle::None;

	auto conv = [&ws](DWORD style, DWORD from, WindowStyle to) {
		if(style & from)
			ws |= to;
	};

	conv(style, WS_BORDER, WindowStyle::Border);
	conv(style, WS_DLGFRAME, WindowStyle::Caption);
	conv(style, WS_DISABLED, WindowStyle::Disabled);
	conv(style, WS_MAXIMIZEBOX, WindowStyle::MaximizeBox);
	conv(style, WS_MINIMIZEBOX, WindowStyle::MinimizeBox);
	conv(style, WS_SIZEBOX, WindowStyle::Sizable);
	conv(style, WS_SYSMENU, WindowStyle::SystemMenu);
	conv(exstyle, WS_EX_CLIENTEDGE, WindowStyle::BorderClientEdge);
	conv(exstyle, WS_EX_STATICEDGE, WindowStyle::BorderStaticEdge);
	conv(exstyle, WS_EX_WINDOWEDGE, WindowStyle::BorderWindowEdge);
	conv(exstyle, WS_EX_TOPMOST, WindowStyle::TopMost);

	return ws;
}

//
///*
// * Window Styles
// */
//#define WS_DISABLED 0x08000000L
//#define WS_BORDER 0x00800000L
//#define WS_SYSMENU 0x00080000L
//#define WS_THICKFRAME 0x00040000L
//#define WS_SIZEBOX WS_THICKFRAME
//#define WS_MINIMIZEBOX 0x00020000L
//#define WS_MAXIMIZEBOX 0x00010000L

//#define WS_DLGFRAME 0x00400000L

//#define WS_POPUP 0x80000000L
//#define WS_CHILD 0x40000000L
//#define WS_VISIBLE 0x10000000L
//#define WS_TABSTOP 0x00010000L

//#define WS_CAPTION 0x00C00000L /* WS_BORDER | WS_DLGFRAME  */

//#define WS_GROUP 0x00020000L

// * Extended Window Styles
// */
//#define WS_EX_DLGMODALFRAME 0x00000001L
//#define WS_EX_NOPARENTNOTIFY 0x00000004L
//#define WS_EX_TOPMOST 0x00000008L
//#define WS_EX_ACCEPTFILES 0x00000010L
//#define WS_EX_TRANSPARENT 0x00000020L
//#if(WINVER >= 0x0400)
//#define WS_EX_MDICHILD 0x00000040L
//#define WS_EX_TOOLWINDOW 0x00000080L
//#define WS_EX_WINDOWEDGE 0x00000100L
//#define WS_EX_CLIENTEDGE 0x00000200L
//#define WS_EX_CONTEXTHELP 0x00000400L
//
//#endif /* WINVER >= 0x0400 */
//#if(WINVER >= 0x0400)
//
//#define WS_EX_RIGHT 0x00001000L
//#define WS_EX_LEFT 0x00000000L
//#define WS_EX_RTLREADING 0x00002000L
//#define WS_EX_LTRREADING 0x00000000L
//#define WS_EX_LEFTSCROLLBAR 0x00004000L
//#define WS_EX_RIGHTSCROLLBAR 0x00000000L
//
//#define WS_EX_CONTROLPARENT 0x00010000L
//#define WS_EX_STATICEDGE 0x00020000L
//#define WS_EX_APPWINDOW 0x00040000L
//
//#define WS_EX_OVERLAPPEDWINDOW (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)
//#define WS_EX_PALETTEWINDOW (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST)
}
