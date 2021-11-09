#include "window_core.hpp"

namespace shoujin::gui {

WindowCore::WindowCore(const LayoutInfo& li) :
	Layout{li},
	_hwnd{},
	_hwnd_parent{}
{
}

WindowCore::WindowCore(WindowCore&& rhs) noexcept :
	_hwnd{rhs._hwnd}, _hwnd_parent{rhs._hwnd_parent}
{
	rhs._hwnd = nullptr;
	rhs._hwnd_parent = nullptr;
}

WindowCore& WindowCore::operator=(WindowCore&& rhs) noexcept
{
	if(this != &rhs) {
		DestroyHandle();

		_hwnd = rhs._hwnd;
		_hwnd_parent = rhs._hwnd_parent;
		rhs._hwnd = nullptr;
		rhs._hwnd_parent = nullptr;
	}

	return *this;
}

WindowCore::~WindowCore()
{
	DestroyHandle();
}

void WindowCore::DestroyHandle()
{
	if(_hwnd) {
		DestroyWindow(_hwnd);
		_hwnd = nullptr;
	}
}

bool WindowCore::OnWndProc(const WindowMessage& message)
{
	switch(message.msg) {
		case WM_CREATE: {
			auto& createinfo = *reinterpret_cast<CREATESTRUCT*>(message.lparam);
			return OnCreate(createinfo);
		}
		case WM_PAINT:
			return OnPaint();
		case WM_DESTROY: {
			_hwnd = nullptr;
			return false;
		}
	}

	return true;
}

void WindowCore::ProcessOnPaintUsingDC(HDC hsourcedc)
{
	PAINTSTRUCT ps;
	BeginPaint(_hwnd, &ps);
	int x = ps.rcPaint.left;
	int y = ps.rcPaint.top;
	int w = ps.rcPaint.right - x;
	int h = ps.rcPaint.bottom - y;
	BitBlt(ps.hdc, x, y, w, h, hsourcedc, x, y, SRCCOPY);
	EndPaint(_hwnd, &ps);
}

}
