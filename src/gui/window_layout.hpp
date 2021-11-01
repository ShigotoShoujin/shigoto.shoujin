#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "types.hpp"

namespace shoujin::gui {
class WindowLayout {
	Point _position;
	Size _window_size, _client_size;
	DWORD _style, _exstyle;

public:
	struct CreateInfo {
		Point position{};
		Size window_size = {};
		Size client_size = {};
		DWORD style = {};
		DWORD exstyle = {};
	};

	WindowLayout();
	WindowLayout(const CreateInfo&);

	[[nodiscard]] inline Point position() const { return _position; }
	[[nodiscard]] inline Size window_size() const { return _window_size; }
	[[nodiscard]] inline Size client_size() const { return _client_size; }
	[[nodiscard]] inline DWORD style() const { return _style; }
	[[nodiscard]] inline DWORD exstyle() const { return _exstyle; }
};
}
