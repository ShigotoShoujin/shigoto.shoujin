#include "../assert/assert.hpp"
#include "../file/file.hpp"
#include "../file/logfile.hpp"
#include "message_string.hpp"
#include "window.hpp"
#include "window_handle.hpp"

namespace shoujin::gui {

WindowHandle::WindowHandle() :
	_hwnd{}
{
}

WindowHandle::~WindowHandle()
{
	Reset();
}

void WindowHandle::CreateHandle(const Window& window)
{
	const LPCTSTR CLASS_NAME = TEXT("ShoujinWindow");
	HINSTANCE hinstance = GetModuleHandle(nullptr);
	WNDCLASSEX wc;

	if(!GetClassInfoEx(hinstance, CLASS_NAME, &wc)) {
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProcStatic;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = CLASS_NAME;
		wc.hIconSm = NULL;

		SHOUJIN_ASSERT_WIN32(RegisterClassEx(&wc));
	}

	HWND hwnd_parent = _parent ? _parent->hwnd() : nullptr;

	SHOUJIN_ASSERT(
		CreateWindowEx(
			window.exstyle(),
			CLASS_NAME,
			CLASS_NAME,
			window.style(),
			window.position().x,
			window.position().y,
			window.window_size().x,
			window.window_size().y,
			hwnd_parent,
			nullptr,
			hinstance,
			this));

	SHOUJIN_ASSERT(_hwnd);

	ShowWindow(_hwnd, SW_SHOW);
	SHOUJIN_ASSERT(UpdateWindow(_hwnd));
}

void WindowHandle::Reset()
{
	if(_hwnd) {
		DestroyWindow(_hwnd);
		_hwnd = nullptr;
	}
}

bool WindowHandle::OnDispatchMessage(const MSG& msg)
{
	return true;
}

bool WindowHandle::OnWndProc(const WindowMessage& message)
{
	switch(message.msg) {
		case WM_DESTROY: {
			_hwnd = nullptr;
			return false;
		}
	}

	return true;
}

void WindowHandle::ProcessOnPaintMessageFromDC(HDC hsourcedc)
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

void WindowHandle::Show()
{
	SHOUJIN_ASSERT(_hwnd);
	MSG msg;

	while(_hwnd && ReadMessageAsync(msg))
		TranslateAndDispatchMessage(msg);
}

void WindowHandle::ShowModal()
{
	SHOUJIN_ASSERT(_hwnd);
	MSG msg;
	while(_hwnd && ReadMessage(msg))
		TranslateAndDispatchMessage(msg);
}

void WindowHandle::Close()
{
	SHOUJIN_ASSERT(_hwnd);
	SendMessage(_hwnd, WM_CLOSE, 0, 0);
}

LRESULT CALLBACK WindowHandle::WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	WindowHandle* window_handle;

	if(!file::FileExists(file::LogFile::GetDebugFile()))
		file::LogFile::AppendDebug(FormatWindowMessageHeader());

	tstring msg_text = FormatWindowMessageLine(hwnd, msg, wparam, lparam);
	file::LogFile::AppendDebug(msg_text);

	if(msg == WM_NCCREATE) {
		CREATESTRUCT& create_struct = *reinterpret_cast<LPCREATESTRUCT>(lparam);
		window_handle = reinterpret_cast<WindowHandle*>(create_struct.lpCreateParams);
		SHOUJIN_ASSERT(window_handle);

		window_handle->_hwnd = hwnd;

		SetLastError(0);
		LONG_PTR previous_value = SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window_handle));
		SHOUJIN_ASSERT_WIN32(previous_value || !GetLastError());
	} else
		window_handle = reinterpret_cast<WindowHandle*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if(window_handle)
		return window_handle->OnWndProc({msg, wparam, lparam}) ? DefWindowProc(hwnd, msg, wparam, lparam) : 0;

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool WindowHandle::ReadMessage(MSG& msg)
{
	auto isok = [](auto result) { return result != -1; }; // GetMessage returns -1 on error
	return SHOUJIN_ASSERT_WIN32_EXPLICIT(GetMessage(&msg, _hwnd, 0, 0), isok);
}

bool WindowHandle::ReadMessageAsync(MSG& msg)
{
	return PeekMessage(&msg, _hwnd, 0, 0, PM_REMOVE);
}

void WindowHandle::TranslateAndDispatchMessage(const MSG& msg)
{
	TranslateMessage(&msg);
	if(OnDispatchMessage(msg))
		DispatchMessage(&msg);
}

}
