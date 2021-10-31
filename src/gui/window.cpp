#include "window.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../assert/assert.hpp"

namespace shoujin::gui {

Window::Window(Window&& rhs) noexcept :
	_hwnd{rhs._hwnd}
{
	rhs._hwnd = nullptr;
}

Window& Window::operator=(Window&& rhs) noexcept
{
	if(this != &rhs) {
		_hwnd = rhs._hwnd;
		rhs._hwnd = nullptr;
	}
	return *this;
}

Window::Window()
{
	_hwnd = nullptr;
}

Window::~Window()
{
	if(_hwnd) {
		DestroyWindow(_hwnd);
		_hwnd = nullptr;
	}
}

bool Window::ProcessMessages()
{
	SHOUJIN_ASSERT(_hwnd);
	MSG msg;

	while(_hwnd && PeekMessage(&msg, _hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return _hwnd;
}

void Window::Show()
{
	if(!_hwnd)
		WIP_Create();

	ProcessMessages();
}

void Window::ShowModal()
{
	MSG msg;

	if(!_hwnd)
		WIP_Create();

	while(_hwnd && SHOUJIN_ASSERT_WIN32_FUNC(GetMessage(&msg, _hwnd, 0, 0), [](auto r) { return r != -1 /*GetMessage returns -1 on error*/; })) {
//The anonymous lambda in SHOUJIN_ASSERT_WIN32 hides the fact that msg is initialized
#pragma warning(suppress : 6001) //Warning C6001 Using uninitialized memory
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT Window::WndProc(UINT msg, WPARAM wparam, LPARAM lparam) noexcept
{
	switch(msg) {
		case WM_DESTROY: {
			_hwnd = nullptr;
			return 0;
		}
	}

	return DefWindowProc(_hwnd, msg, wparam, lparam);
}

void Window::WIP_Create()
{
	const LPCTSTR CLASS_NAME = TEXT("WIP_Create");
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

	SHOUJIN_ASSERT(
		CreateWindowEx(
			WindowLayout::_exstyle,
			CLASS_NAME,
			CLASS_NAME,
			WindowLayout::_style,
			WindowLayout::_position.x,
			WindowLayout::_position.y,
			WindowLayout::_window_size.cx,
			WindowLayout::_window_size.cy,
			HWND_DESKTOP,
			nullptr,
			hinstance,
			this));

	SHOUJIN_ASSERT(_hwnd);

	ShowWindow(_hwnd, SW_SHOW);
	SHOUJIN_ASSERT(UpdateWindow(_hwnd));
}

LRESULT CALLBACK Window::WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
{
	Window* instance;

	if(msg == WM_NCCREATE) {
		CREATESTRUCT& create_struct = *reinterpret_cast<LPCREATESTRUCT>(lparam);
		instance = reinterpret_cast<Window*>(create_struct.lpCreateParams);
		SHOUJIN_ASSERT(instance);

		instance->_hwnd = hwnd;

		SetLastError(0);
		LONG_PTR previous_value = SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
		SHOUJIN_ASSERT_WIN32(previous_value || !GetLastError());
	} else
		instance = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	return instance ? instance->WndProc(msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
}

}
