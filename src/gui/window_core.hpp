#pragma once
#include "layout.hpp"

namespace shoujin::gui {

class WindowCore : public Layout {
protected:
	HWND _hwnd;
	HWND _hwnd_parent;

public:
	struct WindowMessage {
		UINT msg;
		WPARAM wparam;
		LPARAM lparam;
	};

	WindowCore();
	virtual ~WindowCore();

	[[nodiscard]] virtual bool hwnd() const { return _hwnd; }
	[[nodiscard]] virtual bool hwnd_parent() const { return _hwnd_parent; }

	virtual void CreateHandle(const Layout& layout, HWND hwnd_parent) = 0;
	virtual void DestroyHandle();

	virtual bool OnCreate(const CREATESTRUCT& createinfo) { return true; }
	virtual bool OnPaint() { return true; }
	virtual bool OnDispatchMessage(const MSG& msg) { return true; }
	virtual bool OnWndProc(const WindowMessage& message);
	virtual void ProcessOnPaintUsingDC(HDC hsourcedc);

private:
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

}
