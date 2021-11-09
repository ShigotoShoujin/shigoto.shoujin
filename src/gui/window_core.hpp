#pragma once
#include "layout.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

class WindowCore : public Layout {
protected:
	HWND _hwnd;
	HWND _hwnd_parent;

	struct WindowMessage {
		UINT msg;
		WPARAM wparam;
		LPARAM lparam;
	};

	WindowCore(const LayoutInfo& li = {});

public:
	WindowCore(const WindowCore&) = delete;
	WindowCore& operator=(const WindowCore&) = delete;
	WindowCore(WindowCore&&) noexcept;
	WindowCore& operator=(WindowCore&&) noexcept;

	virtual ~WindowCore();

	[[nodiscard]] HWND hwnd() const { return _hwnd; }
	[[nodiscard]] HWND hwnd_parent() const { return _hwnd_parent; }

	virtual void CreateHandle(const Layout& layout, HWND hwnd_parent) = 0;
	virtual void DestroyHandle();

	virtual bool OnCreate(const CREATESTRUCT& createinfo) { return true; }
	virtual bool OnPaint() { return true; }
	virtual bool OnDispatchMessage(const MSG& msg) { return true; }
	virtual bool OnWndProc(const WindowMessage& message);
	virtual void ProcessOnPaintUsingDC(HDC hsourcedc);
};

}
