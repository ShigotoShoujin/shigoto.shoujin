#include "window.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../assert/assert.hpp"
#include "../file/file.hpp"
#include "../file/logfile.hpp"
#include "message_string.hpp"

namespace shoujin::gui {

Window::Window(Window&& rhs) noexcept :
	_hwnd{rhs._hwnd},
	_hparentwnd{rhs._hparentwnd}
{
	rhs._hwnd = nullptr;
	rhs._hparentwnd = nullptr;
}

Window& Window::operator=(Window&& rhs) noexcept
{
	if(this != &rhs) {
		_hwnd = rhs._hwnd;
		_hparentwnd = rhs._hparentwnd;
		rhs._hwnd = nullptr;
		rhs._hparentwnd = nullptr;
	}
	return *this;
}

Window::Window() :
	_hwnd{nullptr},
	_hparentwnd{nullptr}
{
}

Window::Window(const WindowLayout& layout, HWND hparentwnd) :
	WindowLayout{layout},
	_hwnd{nullptr},
	_hparentwnd{hparentwnd}
{
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

	while(_hwnd && PeekMessage(&msg, _hwnd, 0, 0, PM_REMOVE))
		ProcessMessage(msg);

	return _hwnd;
}

void Window::Show()
{
	if(!_hwnd)
		CreateHandle();

	ProcessMessages();
}

void Window::ShowModal()
{
	MSG msg;

	if(!_hwnd)
		CreateHandle();

	auto isok = [](auto r) { return r != -1 /*GetMessage returns -1 on error*/; };
	while(_hwnd && SHOUJIN_ASSERT_WIN32_EXPLICIT(GetMessage(&msg, _hwnd, 0, 0), isok))
#pragma warning(suppress : 6001) //Warning C6001 Using uninitialized memory - The anonymous lambda in SHOUJIN_ASSERT_WIN32 hides the fact that msg is initialized
		ProcessMessage(msg);
}

bool Window::OnDispatchMessage(MSG& msg)
{
	return true;
}

bool Window::OnWndProc(const WindowMessage& message)
{
	switch(message.msg) {
		case WM_DESTROY: {
			_hwnd = nullptr;
			return false;
		}
	}

	return true;
}

void Window::ProcessOnPaintMessageFromDC(HDC hsourcedc)
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

void Window::CreateHandle()
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

	SHOUJIN_ASSERT(
		CreateWindowEx(
			WindowLayout::exstyle(),
			CLASS_NAME,
			CLASS_NAME,
			WindowLayout::style(),
			WindowLayout::position().x,
			WindowLayout::position().y,
			WindowLayout::window_size().x,
			WindowLayout::window_size().y,
			_hparentwnd,
			nullptr,
			hinstance,
			this));

	SHOUJIN_ASSERT(_hwnd);

	ShowWindow(_hwnd, SW_SHOW);
	SHOUJIN_ASSERT(UpdateWindow(_hwnd));
}

LRESULT CALLBACK Window::WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window* instance;

	if(!file::FileExists(file::LogFile::GetDebugFile()))
		file::LogFile::AppendDebug(FormatWindowMessageHeader());

	tstring msg_text = FormatWindowMessageLine(hwnd, msg, wparam, lparam);
	file::LogFile::AppendDebug(msg_text);

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

	if(instance)
		return instance->OnWndProc({msg, wparam, lparam}) ? DefWindowProc(hwnd, msg, wparam, lparam) : 0;

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::ProcessMessage(MSG msg)
{
	TranslateMessage(&msg);
	if(OnDispatchMessage(msg))
		DispatchMessage(&msg);
}

}
