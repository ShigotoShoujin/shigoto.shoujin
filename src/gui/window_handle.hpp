#ifndef SHOUJIN_SOURCE_GUI_WINDOW_HANDLE
#define SHOUJIN_SOURCE_GUI_WINDOW_HANDLE

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>

namespace shoujin::gui {

class WindowHandle {
	HWND _hwnd;
	HWND _parent;

public:
	WindowHandle(HWND hwnd) :
		_hwnd{hwnd},
		_parent{nullptr}
	{}

	WindowHandle(HWND hwnd, HWND parent) :
		_hwnd{hwnd},
		_parent{parent}
	{}

	WindowHandle(WindowHandle&&) = default;
	WindowHandle& operator=(WindowHandle&&) = default;

	~WindowHandle()
	{
		_hwnd = nullptr;
		_parent = nullptr;
	}

	[[nodiscard]] HWND const& hwnd() const { return _hwnd; }
	[[nodiscard]] HWND const& parent() const { return _parent; }

	operator HWND() const { return _hwnd; }
};

}

#endif
