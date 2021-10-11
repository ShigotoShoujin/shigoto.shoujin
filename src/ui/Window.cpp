#include "Window.hpp"
#include <cassert>

const LPCTSTR Window::DEFAULT_CLASS = TEXT("WindowDefaultClass");
const SIZE Window::DEFAULT_SIZE{320, LONG(320 / (16 / 9.0))};

LRESULT CALLBACK Window::WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
{
	Window* self;

	if(msg != WM_NCCREATE && (self = (Window*)(GetWindowLongPtr(hwnd, GWLP_USERDATA))))
		return self->WndProc(msg, wparam, lparam);

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT Window::WndProc(UINT msg, WPARAM wparam, LPARAM lparam) noexcept
{
	switch(msg) {
		case WM_LBUTTONDOWN:
			if(OnMouseClick(wparam, LOWORD(lparam), HIWORD(lparam)))
				return 0;
			break;
		case WM_KEYDOWN:
			if(OnKeyDown(wparam))
				return 0;
			break;
		case WM_DESTROY:
			Destroy();
			return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

Window::Window(Args args) noexcept
{
	HINSTANCE hinstance = GetModuleHandle(NULL);
	WNDCLASSEX wc;

	if(!GetClassInfoEx(hinstance, args.classname, &wc)) {
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
		wc.lpszClassName = args.classname;
		wc.hIconSm = NULL;

		RegisterClassEx(&wc);
	}

	SIZE parent_size;
	if(args.hwnd_parent != HWND_DESKTOP) {
		RECT rect;
		GetClientRect(args.hwnd_parent, &rect);
		parent_size = RectToSize(rect);
		args.style |= WS_CHILD;
	} else
		parent_size = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};

	if(args.mode == Mode::FullScreen) {
		args.position.x = 0;
		args.position.y = 0;
		args.client_size = parent_size;
		args.window_size = parent_size;
		if(args.hwnd_parent == HWND_DESKTOP)
			args.style = WS_POPUP;
	} else {
		//args.style = (args.style ? args.style : WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		args.style = args.style ? args.style : WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;

		if(args.client_size.cx && args.client_size.cy) {
			RECT rect{0, 0, args.client_size.cx, args.client_size.cy};
			AdjustWindowRectEx(&rect, args.style, args.hwnd_menu != 0, args.ex_style);
			args.window_size = RectToSize(rect);
		}

		if(args.mode == Mode::CenterParent) {
			args.position.x = (parent_size.cx - args.window_size.cx) >> 1;
			args.position.y = (parent_size.cy - args.window_size.cy) >> 1;
		}
	}

	hwnd = CreateWindowEx(args.ex_style, args.classname, args.text, args.style, args.position.x, args.position.y, args.window_size.cx, args.window_size.cy, args.hwnd_parent, args.hwnd_menu, hinstance, args.param);
	assert(hwnd);
	destroyed = false;
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(this));
}

void Window::Show() noexcept
{
	assert(!destroyed);
	ShowWindow(hwnd, SW_SHOW);
}

void Window::Hide() noexcept
{
	assert(!destroyed);
	ShowWindow(hwnd, SW_HIDE);
}

bool Window::MessageUpdate() noexcept
{
	assert(!destroyed);

	MSG msg;

	while(!destroyed && PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
		ProcessMessage(msg);

	return !destroyed;
}

bool Window::MessageLoop() noexcept
{
	assert(!destroyed);

	MSG msg;

	while(!destroyed && GetMessage(&msg, hwnd, 0, 0))
		ProcessMessage(msg);

	return !destroyed;
}

void Window::Destroy() noexcept
{
	if(hwnd && !destroyed) {
		destroyed = true;
		DestroyWindow(hwnd);
		hwnd = 0;
	}
}

bool Window::OnMouseClick(WPARAM wparam, int x, int y) noexcept
{
	return false;
}

bool Window::BeforeKeyDown(HWND hwnd, WPARAM wparam) noexcept
{
	return false;
}

bool Window::OnKeyDown(WPARAM wparam) noexcept
{
	return false;
}

Window::~Window() noexcept
{
	Destroy();
}

SIZE Window::GetWindowSize() const noexcept
{
	assert(!destroyed);

	RECT rect;
	GetWindowRect(hwnd, &rect);
	return RectToSize(rect);
}

SIZE Window::GetClientSize() const noexcept
{
	assert(!destroyed);

	RECT rect;
	GetClientRect(hwnd, &rect);
	return RectToSize(rect);
}

tstring Window::GetText() const noexcept
{
	assert(!destroyed);

	int max_count = SendMessage(hwnd, WM_GETTEXTLENGTH, 0, 0);
	TCHAR* buffer = new TCHAR[max_count + 1];
	SendMessage(hwnd, WM_GETTEXT, max_count + 1, (LPARAM)buffer);

	tstring text(buffer);
	delete[] buffer;

	return text;
}

void Window::ProcessMessage(const MSG& msg) noexcept
{
	TranslateMessage(&msg);

	if(msg.message == WM_KEYDOWN)
		if(BeforeKeyDown(msg.hwnd, msg.wParam))
			return;

	DispatchMessage(&msg);
}

inline SIZE RectToSize(const RECT& rect)
{
	return {rect.right - rect.left, rect.bottom - rect.top};
}
