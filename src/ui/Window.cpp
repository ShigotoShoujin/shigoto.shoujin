#include "Window.hpp"
#include <cassert>

constexpr LONG DEFAULT_WIDTH = 384;
constexpr SIZE GetDefaultSize(const int size) { return {size, LONG(size / (16 / 9.0))}; }

static LPCTSTR DEFAULT_CLASS_NAME = TEXT("ShigotoShoujinWndClass");
const DWORD Window::DEFAULT_STYLE = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;

static void PrepareWndClass(HINSTANCE hinstance, LPCTSTR class_name, WNDPROC wnd_proc) noexcept;
static SIZE GetParentSize(HWND hwnd_parent) noexcept;
static SIZE AdjustWindowSize(SIZE client_size, DWORD style, DWORD ex_style) noexcept;
static POINT CenterWindow(SIZE parent_size, SIZE window_size) noexcept;

Window::Window(Window&& other) noexcept :
	active{other.active},
	hwnd{other.hwnd},
	position{other.position},
	window_size{other.window_size},
	style{other.style}
{
	other.hwnd = {};
	other.active = false;
}

void Window::CreateHandle(const WindowCreateInfo& wci) noexcept
{
	HINSTANCE hinstance = GetModuleHandle(NULL);
	LPCTSTR class_name = wci.class_name ? wci.class_name : DEFAULT_CLASS_NAME;

	position = wci.position;
	window_size = wci.window_size;
	style = wci.style;

	bool is_child = wci.style & WS_CHILD;
	assert((is_child && wci.hwnd_parent) || (!is_child && !wci.hwnd_parent));

	if(!style && !wci.class_name)
		style = DEFAULT_STYLE;

	if(wci.hwnd_parent != HWND_DESKTOP)
		style |= WS_CHILD;

	PrepareWndClass(hinstance, class_name, Window::WndProcStatic);

	switch(wci.layout) {
		case Layout::Custom:
		case Layout::CenterParent:
			if(wci.client_size.cx && wci.client_size.cy)
				window_size = AdjustWindowSize(wci.client_size, style, wci.ex_style);
			else if(!window_size.cx || !window_size.cy)
				window_size = GetDefaultSize(DEFAULT_WIDTH);

			if(wci.layout == Layout::CenterParent)
				position = CenterWindow(GetParentSize(wci.hwnd_parent), window_size);

			break;
		case Layout::FillParent:
			if(!wci.style && wci.hwnd_parent == HWND_DESKTOP)
				style = WS_POPUP;
			position = {};
			window_size = GetParentSize(wci.hwnd_parent);
	}

	//Win32 common controls use their own WndProc so hwnd and active must be assigned here as well
	hwnd = CreateWindowEx(wci.ex_style, class_name, wci.text, style, position.x, position.y, window_size.cx, window_size.cy, wci.hwnd_parent, wci.hwnd_menu, hinstance, this);
	assert(hwnd);
	active = true;
}

Window::Window(const WindowCreateInfo& wci) noexcept
{
	initial_wci = wci;
	CreateHandle(wci);
}


Window::~Window() noexcept
{
	Destroy();
}

void Window::Show() noexcept
{
	assert(active);
	ShowWindow(hwnd, SW_SHOW);
}

void Window::Hide() noexcept
{
	assert(active);
	ShowWindow(hwnd, SW_HIDE);
}

bool Window::MessageUpdate() noexcept
{
	assert(active);

	MSG msg;

	while(active && PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
		ProcessMessage(msg);

	return active;
}

bool Window::MessageLoop() noexcept
{
	assert(active);

	MSG msg;

	while(active && GetMessage(&msg, hwnd, 0, 0))
		ProcessMessage(msg);

	return active;
}

void Window::Destroy() noexcept
{
	if(hwnd && active) {
		active = false;
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

SIZE Window::GetWindowSize() const noexcept
{
	assert(active);

	RECT rect;
	GetWindowRect(hwnd, &rect);
	return RectToSize(rect);
}

SIZE Window::GetClientSize() const noexcept
{
	assert(active);

	RECT rect;
	GetClientRect(hwnd, &rect);
	return RectToSize(rect);
}

tstring Window::GetText() const noexcept
{
	assert(active);

	size_t max_count = SendMessage(hwnd, WM_GETTEXTLENGTH, 0, 0);
	TCHAR* buffer = new TCHAR[max_count + 1];
	SendMessage(hwnd, WM_GETTEXT, max_count + 1, (LPARAM)buffer);

	tstring text(buffer);
	delete[] buffer;

	return text;
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

LRESULT CALLBACK Window::WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
{
	Window* self;

	if(msg == WM_NCCREATE) {
		CREATESTRUCT& cs = *(LPCREATESTRUCT)lparam;
		self = (Window*)cs.lpCreateParams;
		assert(self);

		self->active = true;
		self->hwnd = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)self);
	} else
		self = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if(self)
		return self->WndProc(msg, wparam, lparam);
	else
		return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::ProcessMessage(const MSG& msg) noexcept
{
	TranslateMessage(&msg);

	if(msg.message == WM_KEYDOWN)
		if(BeforeKeyDown(msg.hwnd, msg.wParam))
			return;

	DispatchMessage(&msg);
}

inline SIZE RectToSize(const RECT& rect) noexcept
{
	return {rect.right - rect.left, rect.bottom - rect.top};
}

static void PrepareWndClass(HINSTANCE hinstance, LPCTSTR class_name, WNDPROC wnd_proc) noexcept
{
	WNDCLASSEX wc;

	if(!GetClassInfoEx(hinstance, class_name, &wc)) {
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = wnd_proc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = class_name;
		wc.hIconSm = NULL;

		RegisterClassEx(&wc);
	}
}

static SIZE GetParentSize(HWND hwnd_parent) noexcept
{
	if(hwnd_parent != HWND_DESKTOP) {
		RECT rect;
		GetClientRect(hwnd_parent, &rect);
		return RectToSize(rect);
	} else
		return {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
}

static SIZE AdjustWindowSize(SIZE client_size, DWORD style, DWORD ex_style) noexcept
{
	RECT rect{0, 0, client_size.cx, client_size.cy};
	AdjustWindowRectEx(&rect, style, 0, ex_style);
	return RectToSize(rect);
}

static POINT CenterWindow(SIZE parent_size, SIZE window_size) noexcept
{
	return {
		(parent_size.cx - window_size.cx) >> 1,
		(parent_size.cy - window_size.cy) >> 1};
}
