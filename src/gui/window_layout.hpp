#pragma once
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {
class WindowLayout {
protected:
	POINT _position;
	SIZE _client_size, _window_size;
	DWORD _style, _exstyle;

public:
	WindowLayout(HWND hparentwnd = nullptr);

	[[nodiscard]] inline POINT Position() const { return _position; }
	[[nodiscard]] inline SIZE ClientSize() const { return _client_size; }
	[[nodiscard]] inline SIZE WindowSize() const { return _window_size; }
	[[nodiscard]] inline DWORD Style() const { return _style; }
	[[nodiscard]] inline DWORD ExStyle() const { return _exstyle; }
};
}
