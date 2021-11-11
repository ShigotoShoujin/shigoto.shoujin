#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>

namespace shoujin::gui {

class WindowHandle {
	HWND _hwnd;
	const WindowHandle* _parent;

public:
	WindowHandle(HWND hwnd) :
		_hwnd{hwnd},
		_parent{nullptr}
	{}

	WindowHandle(HWND hwnd, WindowHandle* parent) :
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

	[[nodiscard]] const HWND& hwnd() const { return _hwnd; }
	[[nodiscard]] const WindowHandle* parent() const { return _parent; }

	operator HWND() const { return _hwnd; }
};

}
