#include "../assert/assert.hpp"
#include "logging/message_logger.hpp"
#include "window.hpp"

template<typename T> static T GetWindowPtr(HWND hWnd, int nIndex);
static LONG_PTR SetWindowPtr(HWND hWnd, int nIndex, auto dwNewLong);

namespace shoujin::gui {

Window::Window(const LayoutParam& lp) :
	Layout{lp}
{
}

Window::Window(const Window& rhs) :
	Layout{rhs}
{
	//TODO See if the default copy ctor is enough
}

Window& Window::operator=(const Window& rhs)
{
	//TODO See if the default copy assignment operator is enough

	if(this != &rhs) {
		// TODO Trace this
		Layout::Layout(rhs);
	}

	return *this;
}

bool Window::ProcessMessageQueue()
{
	SHOUJIN_ASSERT(!!_handle);

	MSG msg;
	while(_handle && ReadMessageAsync(msg))
		TranslateAndDispatchMessage(msg);

	return !!_handle;
}

void Window::Show()
{
	if(!_handle)
		CreateHandle();

	ShowWindow(*_handle, SW_SHOW);
	SHOUJIN_ASSERT(UpdateWindow(*_handle));

	ProcessMessageQueue();
}

void Window::ShowModal()
{
	Show();

	MSG msg;
	while(_handle && ReadMessage(msg))
		TranslateAndDispatchMessage(msg);
}

void Window::CreateHandle(const WindowHandle* parent)
{
	CreateParam cp = OnCreateParam();
	HINSTANCE hinstance = GetModuleHandle(nullptr);
	WNDCLASSEX wc;

	if(!GetClassInfoEx(hinstance, cp.classname, &wc)) {
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
		wc.lpszClassName = cp.classname;
		wc.hIconSm = NULL;

		SHOUJIN_ASSERT_WIN32(RegisterClassEx(&wc));
	}

	DWORD style = Layout::style();
	if(parent)
		style |= WS_CHILD;
	else if(!style)
		style = Window::DefaultStyle;

	auto pos = position();
	auto size = window_size();
	auto hwnd_parent = parent ? parent->hwnd() : nullptr;
	HWND hwnd = SHOUJIN_ASSERT(CreateWindowEx(exstyle(), cp.classname, cp.classname, style, pos.x, pos.y, size.x, size.y, hwnd_parent, nullptr, hinstance, this));

	//TODO Do we have a comctrl32 ?
	SHOUJIN_ASSERT(hwnd && _handle);
}

Window::CreateParam Window::OnCreateParam()
{
	return {.classname = TEXT("ShoujinWindow")};
}

bool Window::OnDispatchMessage(const MSG& msg)
{
	return true;
}

bool Window::OnWndProc(const WindowMessage& message)
{
	switch(message.msg) {
		case WM_CREATE: {
			auto& createparam = *reinterpret_cast<CREATESTRUCT*>(message.lparam);
			bool handled = OnCreate(createparam);
			OnCreateEvent(*this, createparam);
			return handled;
		}
		case WM_DESTROY:
			_handle.release();
			return kMsgHandled;
	}

	return kMsgNotHandled;
}

bool Window::OnCreate(const CREATESTRUCT& createparam)
{
	return kMsgNotHandled;
}

LRESULT CALLBACK Window::WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window* self;

	logging::LogMessage(hwnd, msg, wparam, lparam);

	if(msg == WM_NCCREATE)
	{
		CREATESTRUCT& createparam = *reinterpret_cast<LPCREATESTRUCT>(lparam);
		self = SHOUJIN_ASSERT(reinterpret_cast<Window*>(createparam.lpCreateParams));
		self->_handle = std::make_unique<WindowHandle>(hwnd);

		SetWindowPtr(hwnd, GWLP_USERDATA, self);
	}
	else
		self = GetWindowPtr<Window*>(hwnd, GWLP_USERDATA);

	if(self)
		if(self->OnWndProc({msg, wparam, lparam}))
			return DefWindowProc(hwnd, msg, wparam, lparam);
		else
			return kMsgHandled;

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool Window::ReadMessage(MSG& msg)
{
	SHOUJIN_ASSERT(!!_handle);
	auto isok = [](auto result) { return result != -1; }; // GetMessage returns -1 on error
	return SHOUJIN_ASSERT_WIN32_EXPLICIT(GetMessage(&msg, *_handle, 0, 0), isok);
}

bool Window::ReadMessageAsync(MSG& msg)
{
	SHOUJIN_ASSERT(!!_handle);
	return PeekMessage(&msg, *_handle, 0, 0, PM_REMOVE);
}

void Window::TranslateAndDispatchMessage(const MSG& msg)
{
	TranslateMessage(&msg);
	if(OnDispatchMessage(msg))
		DispatchMessage(&msg);
}

const bool Window::kMsgHandled = false;
const bool Window::kMsgNotHandled = true;

}

template<typename T> static T GetWindowPtr<T>(HWND hWnd, int nIndex)
{
	return reinterpret_cast<T>(GetWindowLongPtr(hWnd, nIndex));
}

static LONG_PTR SetWindowPtr(HWND hWnd, int nIndex, auto dwNewLong)
{
	SetLastError(0);
	LONG_PTR previous_value = SetWindowLongPtr(hWnd, nIndex, reinterpret_cast<LONG_PTR>(dwNewLong));
	SHOUJIN_ASSERT_WIN32(previous_value || !GetLastError());
	return previous_value;
}
