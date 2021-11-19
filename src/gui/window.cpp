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

static Rect GetOnSizingMinRect(WPARAM wparam, const Rect& onsizing_rect, const Size& min_size);

Window::Window(const LayoutParam& lp) :
	Layout{lp},
	_default_wndproc{nullptr},
	_taborder{}
{
}

Window::Window(const Window& rhs) :
	Layout{rhs},
	_default_wndproc{nullptr},
	_taborder{}
{
	CopyChilds(rhs);
}

Window& Window::operator=(const Window& rhs)
{
	if(this == &rhs)
		return *this;

	Destroy();

	Layout::operator=(rhs);
	_default_wndproc = rhs._default_wndproc;
	_taborder = rhs._taborder;

	CopyChilds(rhs);

	return *this;
}

/// <summary>
/// Return null when the specified hwnd is not found or not in the same instance
/// </summary>
Window* Window::FindWindowByHandle(HWND hwnd)
{
	HINSTANCE hinstance = SHOUJIN_ASSERT_WIN32(GetModuleHandle(nullptr));
	HINSTANCE hwnd_hinstance = SHOUJIN_ASSERT(GetWindowPtr<HINSTANCE>(hwnd, GWLP_HINSTANCE));

	if(hinstance != hwnd_hinstance)
		return nullptr;

	Window* window = SHOUJIN_ASSERT(GetWindowPtr<Window*>(hwnd, GWLP_USERDATA));

	return window;
}

void Window::AddChild(Window* child)
{
	SHOUJIN_ASSERT(child);
	_child_vec.emplace_back(child);
}

Window* Window::GetChild(int index)
{
	if(index >= 0 && index < _child_vec.size())
		return &*_child_vec[index];

	return nullptr;
}

void Window::Close()
{
	if(_handle)
		SendMessage(*_handle, WM_CLOSE, 0, 0);
}

void Window::Destroy()
{
	if(_handle)
		PostMessage(*_handle, WM_DESTROY, 0, 0);
}

bool Window::ProcessMessageQueue()
{
	SHOUJIN_ASSERT(!!_handle);

	MSG msg;
	while(_handle && ReadMessageAsync(msg))
		TranslateAndDispatchMessage(msg);

	return !!_handle;
}

/// <summary>
/// If the window has childs, set focus to the first.
/// Else set focus to itself.
/// </summary>
void Window::SetFocus()
{
	HWND hwnd = _child_vec.size() ? *_child_vec[0]->_handle : *_handle;
	SHOUJIN_ASSERT_WIN32(::SetFocus(hwnd));
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
	ConstructWindow(parent);
	_window_group = std::make_unique<WindowTabOrder>();

	for(auto& child : _child_vec) {
		child->CreateHandle(_handle.get());
		_window_group->AddWindow(&*child, child->_taborder);
	}

	RaiseOnParentSized();

	SetFocus();
}

Window::CreateParam Window::OnCreateParam()
{
	return {.classname = TEXT("ShoujinWindow")};
}

bool Window::OnDispatchMessage(const MSG& msg)
{
	if(msg.message == WM_KEYDOWN && msg.wParam == VK_TAB) {
		bool cycle_up = GetAsyncKeyState(VK_SHIFT);
		_window_group->CycleTab(cycle_up);
		return true;
	}

	return false;
}

bool Window::OnWndProc(const WindowMessage& message)
{
	switch(message.msg) {
		case WM_CREATE:
			return RaiseOnCreate(message);
		case WM_CLOSE:
			return RaiseOnClose();
		case WM_PAINT:
			return RaiseOnPaint();
		case WM_SIZING:
			return RaiseOnSizing(message);
		case WM_EXITSIZEMOVE:
			return RaiseOnSizingFinished();
		case WM_DESTROY:
			return RaiseOnDestroy();
	}

	return false;
}

bool Window::OnCreate(const CREATESTRUCT& createparam)
{
	return false;
}

bool Window::OnClose()
{
	return false;
}

bool Window::OnPaint()
{
	return false;
}

bool Window::OnSizing(WPARAM wparam, Rect* onsizing_rect)
{
	*onsizing_rect = GetOnSizingMinRect(wparam, *onsizing_rect, {512, 512});
	Layout::UpdateWindowSize(RectToSize(*onsizing_rect));
	return true;
}

bool Window::OnSizingFinished()
{
	return false;
}

void Window::OnParentSized(const Window& parent)
{
	//Integrate this func in Layout Stream ?
	constexpr int margin = 11;

	Size ps = parent.client_size();
	Rect rect = window_rect();
	Rect new_rect = rect;

	int la = anchor();

	if(la & AnchorLeft) {
		new_rect.x1 = margin;
		new_rect.x2 = new_rect.x1 + rect.width();
	}

	if(la & AnchorTop) {
		new_rect.y1 = margin;
		new_rect.y2 = new_rect.y1 + rect.height();
	}

	if(la & AnchorRight) {
		new_rect.x1 = ps.x - rect.width() - margin;
		new_rect.x2 = new_rect.x1 + rect.width();
	}

	if(la & AnchorBottom) {
		new_rect.y1 = ps.y - rect.height() - margin;
		new_rect.y2 = new_rect.y1 + rect.height();
	}

	if(new_rect != rect) {
		MoveWindow(*_handle, new_rect.x1, new_rect.y1, new_rect.width(), new_rect.height(), TRUE);
		Layout::UpdateFromHandle(*_handle);
	}
}

void Window::OnDestroy()
{
}

Window::MessageResult Window::RaiseOnCreate(const WindowMessage& message)
{
	auto& createparam = *reinterpret_cast<CREATESTRUCT*>(message.lparam);
	return OnCreate(createparam) | (OnCreateEvent ? OnCreateEvent(*this, createparam) : false);
}

Window::MessageResult Window::RaiseOnDispatchMessage(const MSG& msg)
{
	return OnDispatchMessage(msg) | (OnDispatchMessageEvent ? OnDispatchMessageEvent(msg) : false);
}

Window::MessageResult Window::RaiseOnWndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	WindowMessage wmsg{msg, wparam, lparam};
	return OnWndProc(wmsg) | (OnWndProcEvent ? OnWndProcEvent(wmsg) : false);
}

Window::MessageResult Window::RaiseOnClose()
{
	return OnClose() | (OnCloseEvent ? OnCloseEvent() : false);
}

Window::MessageResult Window::RaiseOnPaint()
{
	return OnPaint() | (OnPaintEvent ? OnPaintEvent() : false);
}

Window::MessageResult Window::RaiseOnSizing(const WindowMessage& message)
{
	RECT* sizing_rect = reinterpret_cast<RECT*>(message.lparam);
	Rect new_rect = *sizing_rect;

	MessageResult onsizing_handled = OnSizing(message.wparam, &new_rect) | (OnSizingEvent ? OnSizingEvent(message.wparam, &new_rect) : false);

	if(onsizing_handled)
		*sizing_rect = new_rect;

	RaiseOnParentSized();

	//WM_SIZING - An application should return TRUE if it processes this message
	return MessageResult{true, TRUE};
}

Window::MessageResult Window::RaiseOnSizingFinished()
{
	return OnSizingFinished() | (OnSizingFinishedEvent ? OnSizingFinishedEvent() : false);
}

Window::MessageResult Window::RaiseOnDestroy()
{
	OnDestroy();
	OnDestroyEvent();

	_handle.release();
	_window_group.release();

	return true;
}

void Window::RaiseOnParentSized()
{
	for(auto& child : _child_vec)
		child->OnParentSized(*this);
}

Window* Window::Clone() const
{
	return new Window(*this);
}

void Window::CopyChilds(const Window& rhs)
{
	_child_vec.reserve(rhs._child_vec.size());
	for(auto& child : rhs._child_vec) {
		auto clone = child->Clone();
		_child_vec.emplace_back(clone);
	}
}

void Window::ConstructWindow(const WindowHandle* parent)
{
	CreateParam cp = OnCreateParam();
	HINSTANCE hinstance = SHOUJIN_ASSERT_WIN32(GetModuleHandle(nullptr));
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

	Layout::UpdateStyle(style, exstyle());

	auto pos = position();
	auto size = window_size();
	auto hwnd_parent = parent ? parent->hwnd() : nullptr;
	HWND hwnd = SHOUJIN_ASSERT(CreateWindowEx(exstyle(), cp.classname, text().c_str(), style, pos.x, pos.y, size.x, size.y, hwnd_parent, nullptr, hinstance, this));

	if(cp.need_subclassing) {
		_handle = std::make_unique<WindowHandle>(hwnd, hwnd_parent);
		SetWindowPtr(hwnd, GWLP_USERDATA, this);
		_default_wndproc = SetWindowPtr(hwnd, GWLP_WNDPROC, WndProcSubclassStatic);
	}

	SHOUJIN_ASSERT(hwnd && _handle);
	Layout::UpdateFromHandle(hwnd);
}

LRESULT CALLBACK Window::WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window* self;

	logging::LogWndProcMessage(hwnd, msg, wparam, lparam);

	if(msg == WM_NCCREATE) {
		CREATESTRUCT& createparam = *reinterpret_cast<LPCREATESTRUCT>(lparam);
		self = SHOUJIN_ASSERT(reinterpret_cast<Window*>(createparam.lpCreateParams));
		self->_handle = std::make_unique<WindowHandle>(hwnd, createparam.hwndParent);

		SetWindowPtr(hwnd, GWLP_USERDATA, self);
	} else
		self = GetWindowPtr<Window*>(hwnd, GWLP_USERDATA);

	if(self) {
		auto message_result = self->RaiseOnWndProc(msg, wparam, lparam);
		return message_result ? message_result.ret_code : DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK Window::WndProcSubclassStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	logging::LogWndProcMessage(hwnd, msg, wparam, lparam);
	Window* self = SHOUJIN_ASSERT(GetWindowPtr<Window*>(hwnd, GWLP_USERDATA));

	auto message_result = self->RaiseOnWndProc(msg, wparam, lparam);
	return message_result ? message_result.ret_code : CallWindowProc(self->_default_wndproc, hwnd, msg, wparam, lparam);
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
	if(!RaiseOnDispatchMessage(msg))
		DispatchMessage(&msg);
}

static Rect GetOnSizingMinRect(WPARAM wparam, const Rect& onsizing_rect, const Size& min_size)
{
	Rect rect = onsizing_rect;
	Size offset = {rect.width() - min_size.x, rect.height() - min_size.y};

	if(offset.x < 0)
		if(wparam == WMSZ_LEFT || wparam == WMSZ_TOPLEFT || wparam == WMSZ_BOTTOMLEFT)
			rect.x1 += offset.x;
		else
			rect.x2 -= offset.x;

	if(offset.y < 0)
		if(wparam == WMSZ_TOP || wparam == WMSZ_TOPLEFT || wparam == WMSZ_TOPRIGHT)
			rect.y1 += offset.y;
		else
			rect.y2 -= offset.y;

	return rect;
}

}
