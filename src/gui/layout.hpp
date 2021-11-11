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

struct LayoutParam {
	LayoutMode create_mode{};
	Point position{};
	Size window_size{};
	Size client_size{};
	DWORD style{};
	DWORD exstyle{};
	bool tabstop{true};
};

class Layout {
	Point _position;
	Size _window_size;
	Size _client_size;
	DWORD _style;
	DWORD _exstyle;
	bool _tabstop;

public:
	static constexpr DWORD DefaultStyle = WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX;

	Layout(const LayoutParam& = {});

	[[nodiscard]] const Point& position() const { return _position; }
	[[nodiscard]] const Size& window_size() const { return _window_size; }
	[[nodiscard]] const Size& client_size() const { return _client_size; }
	[[nodiscard]] const DWORD& style() const { return _style; }
	[[nodiscard]] const DWORD& exstyle() const { return _exstyle; }
	[[nodiscard]] const bool& tabstop() const { return _tabstop; }
};

}
