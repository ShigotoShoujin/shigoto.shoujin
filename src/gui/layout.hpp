#ifndef SHOUJIN_SOURCE_GUI_LAYOUT
#define SHOUJIN_SOURCE_GUI_LAYOUT

#include "../tstring.hpp"
#include "types.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

enum class LayoutMode {
	Default,
	CenterParent,
	FillParent
};

constexpr int AnchorNone = 0;
constexpr int AnchorLeft = 1;
constexpr int AnchorTop = 2;
constexpr int AnchorRight = 4;
constexpr int AnchorBottom = 8;

struct LayoutParam {
	LayoutMode layout_mode{};
	Point position{};
	Size window_size{};
	Size client_size{};
	DWORD style{};
	DWORD exstyle{};
	int anchor{};
	bool tabstop{true};
	tstring text{};
};

class Layout {
	Point _position;
	Size _window_size;
	Size _client_size;
	DWORD _style;
	DWORD _exstyle;
	int _anchor;
	bool _tabstop;
	tstring _text;

public:
	static constexpr DWORD DefaultStyle = WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX;

	Layout(LayoutParam const& = {});

	[[nodiscard]] Point const& position() const { return _position; }
	[[nodiscard]] Size const& window_size() const { return _window_size; }
	[[nodiscard]] Size const& client_size() const { return _client_size; }
	[[nodiscard]] DWORD const& style() const { return _style; }
	[[nodiscard]] DWORD const& exstyle() const { return _exstyle; }
	[[nodiscard]] int anchor() const { return _anchor; }
	[[nodiscard]] bool const& tabstop() const { return _tabstop; }
	[[nodiscard]] tstring const& text() const { return _text; }
	[[nodiscard]] Rect window_rect() const { return Rect{_position, _window_size}; }

	void UpdateStyle(DWORD style, DWORD exstyle);
	void UpdateWindowSize(Size const& window_size);
	void UpdateRectFromHandle(HWND hwnd);
	void UpdateFromHandle(HWND hwnd);
};

}

#endif
