#pragma once
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
};

class Layout {
	Point _position;
	Size _window_size;
	Size _client_size;
	DWORD _style;
	DWORD _exstyle;
	int _anchor;
	bool _tabstop;

public:
	static constexpr DWORD DefaultStyle = WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX;

	Layout(const LayoutParam& = {});

	[[nodiscard]] const Point& position() const { return _position; }
	[[nodiscard]] const Size& window_size() const { return _window_size; }
	[[nodiscard]] const Size& client_size() const { return _client_size; }
	[[nodiscard]] const DWORD& style() const { return _style; }
	[[nodiscard]] const DWORD& exstyle() const { return _exstyle; }
	[[nodiscard]] int anchor() const { return _anchor; }
	[[nodiscard]] const bool& tabstop() const { return _tabstop; }
	[[nodiscard]] Rect window_rect() const { return Rect{_position, _window_size}; }

	void UpdateWindowSize(const Size& window_size);
	void UpdateRectFromHandle(HWND hwnd);
	void UpdateFromHandle(HWND hwnd);
};

}
