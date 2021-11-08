#pragma once
#include "types.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

class Layout {
	Point _position;
	Size _window_size;
	Size _client_size;
	DWORD _style;
	DWORD _exstyle;

public:
	static constexpr DWORD DefaultStyle = WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX;

	enum class CreateMode {
		Default,
		CenterParent,
		FillParent
	};

	struct LayoutInfo {
		CreateMode create_mode{};
		Point position{};
		Size window_size{};
		Size client_size{};
		DWORD style{};
		DWORD exstyle{};
	};

	Layout(const LayoutInfo& li = {});

	[[nodiscard]] const Point& position() const { return _position; }
	[[nodiscard]] const int& left() const { return _position.x; }
	[[nodiscard]] const int& top() const { return _position.y; }
	[[nodiscard]] const Size& window_size() const { return _window_size; }
	[[nodiscard]] const int& window_width() const { return _window_size.x; }
	[[nodiscard]] const int& window_height() const { return _window_size.y; }
	[[nodiscard]] const Size& client_size() const { return _client_size; }
	[[nodiscard]] const int& client_width() const { return _client_size.x; }
	[[nodiscard]] const int& client_height() const { return _client_size.y; }
	[[nodiscard]] const DWORD& style() const { return _style; }
	[[nodiscard]] const DWORD& exstyle() const { return _exstyle; }
};

}
