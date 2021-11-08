#include "../assert/assert.hpp"
#include "../file/file.hpp"
#include "../file/logfile.hpp"
#include "message_string.hpp"
#include "window_core.hpp"

namespace shoujin::gui {

WindowCore::WindowCore() :
	_hwnd{},
	_hwnd_parent{}
{
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

LRESULT CALLBACK WindowCore::WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	WindowCore* window_core;

	if(!file::FileExists(file::LogFile::GetDebugFile()))
		file::LogFile::AppendDebug(FormatWindowMessageHeader());

	tstring msg_text = FormatWindowMessageLine(hwnd, msg, wparam, lparam);
	file::LogFile::AppendDebug(msg_text);

	if(msg == WM_NCCREATE) {
		CREATESTRUCT& create_struct = *reinterpret_cast<LPCREATESTRUCT>(lparam);
		window_core = SHOUJIN_ASSERT(reinterpret_cast<WindowCore*>(create_struct.lpCreateParams));
		window_core->_hwnd = hwnd;

		SetLastError(0);
		LONG_PTR previous_value = SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window_core));
		SHOUJIN_ASSERT_WIN32(previous_value || !GetLastError());
	} else
		window_core = reinterpret_cast<WindowCore*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if(window_core)
		if(window_core->OnWndProc({msg, wparam, lparam}))
			return DefWindowProc(hwnd, msg, wparam, lparam);
		else
			return 0; //The message was already processed by an override and DefWindowProc should not be called

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

}
