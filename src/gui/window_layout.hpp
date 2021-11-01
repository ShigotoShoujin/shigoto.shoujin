#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {
class WindowLayout {
	POINT _position;
	SIZE _window_size, _client_size;
	DWORD _style, _exstyle;

public:
	struct CreateInfo {
		POINT position{};
		SIZE window_size = {};
		SIZE client_size = {};
		DWORD style = {};
		DWORD exstyle = {};
	};

	WindowLayout();
	WindowLayout(const CreateInfo&);

	[[nodiscard]] inline POINT position() const { return _position; }
	[[nodiscard]] inline SIZE window_size() const { return _window_size; }
	[[nodiscard]] inline SIZE client_size() const { return _client_size; }
	[[nodiscard]] inline DWORD style() const { return _style; }
	[[nodiscard]] inline DWORD exstyle() const { return _exstyle; }
};
}
