#include "../assert/assert.hpp"
#include "logging/message_logger.hpp"
#include "window.hpp"

template<typename T> static T GetWindowPtr(HWND hwnd, int index)
{
	return reinterpret_cast<T>(GetWindowLongPtr(hwnd, index));
}

static auto SetWindowPtr(HWND hwnd, int index, auto new_value) -> decltype(new_value)
{
	SetLastError(0);
	LONG_PTR previous_value = SetWindowLongPtr(hwnd, index, reinterpret_cast<LONG_PTR>(new_value));
	SHOUJIN_ASSERT_WIN32(previous_value || !GetLastError());
	return reinterpret_cast<decltype(new_value)>(previous_value);
}

namespace shoujin::gui {

Window::Window(const LayoutParam& lp) :
	Layout{lp},
	_default_wndproc{nullptr}
{
}

Window::Window(const Window& rhs) :
	Layout{rhs},
	_default_wndproc{nullptr}
{
	_childs.reserve(rhs._childs.size());
	for(auto& child : rhs._childs) {
		_childs.emplace_back(new Window(*child));
	}
}

Window& Window::operator=(const Window& rhs)
{
	// TODO Finish this func

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

void Window::AddChild(Window* child)
{
	SHOUJIN_ASSERT(child);
	_childs.emplace_back(child);
}

void Window::CreateHandle(const WindowHandle* parent)
{
	CreateParam cp = OnCreateParam();
	HINSTANCE hinstance = GetModuleHandle(nullptr);
	WNDCLASSEX wc;

	if(!cp.need_subclassing && !GetClassInfoEx(hinstance, cp.classname, &wc)) {
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
		style |= WS_CHILD | WS_VISIBLE;
	else if(!style)
		style = Window::DefaultStyle;

	auto pos = position();
	auto size = window_size();
	auto hwnd_parent = parent ? parent->hwnd() : nullptr;
	HWND hwnd = SHOUJIN_ASSERT(CreateWindowEx(exstyle(), cp.classname, cp.classname, style, pos.x, pos.y, size.x, size.y, hwnd_parent, nullptr, hinstance, this));

	if(cp.need_subclassing) {
		_handle = std::make_unique<WindowHandle>(hwnd, hwnd_parent);
		SetWindowPtr(hwnd, GWLP_USERDATA, this);
		_default_wndproc = SetWindowPtr(hwnd, GWLP_WNDPROC, WndProcSubclassStatic);
	}

	SHOUJIN_ASSERT(hwnd && _handle);

	for(auto& child : _childs)
		child->CreateHandle(_handle.get());
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
			return OnCreate(createparam) && OnCreateEvent(*this, createparam);
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

	if(msg == WM_NCCREATE) {
		CREATESTRUCT& createparam = *reinterpret_cast<LPCREATESTRUCT>(lparam);
		self = SHOUJIN_ASSERT(reinterpret_cast<Window*>(createparam.lpCreateParams));
		self->_handle = std::make_unique<WindowHandle>(hwnd, createparam.hwndParent);

		SetWindowPtr(hwnd, GWLP_USERDATA, self);
	} else
		self = GetWindowPtr<Window*>(hwnd, GWLP_USERDATA);

	if(self)
		if(self->OnWndProc({msg, wparam, lparam}))
			return DefWindowProc(hwnd, msg, wparam, lparam);
		else
			return kMsgHandled;

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK Window::WndProcSubclassStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	logging::LogMessage(hwnd, msg, wparam, lparam);
	Window* self = SHOUJIN_ASSERT(GetWindowPtr<Window*>(hwnd, GWLP_USERDATA));

	if(self->OnWndProc({msg, wparam, lparam}))
		return CallWindowProc(self->_default_wndproc, hwnd, msg, wparam, lparam);

	return kMsgHandled;
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
