module;
#include "../../assert/assert.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Window : Core;
import : Layout;
import : Point;
import : Size;
import : Style;
import Shoujin.Event;
import Shoujin.String;
import Shoujin.Win32Api;

template<typename T> static T GetWindowPtr(HWND hWnd, int index)
{
	return reinterpret_cast<T>(GetWindowLongPtr(hWnd, index));
}

static auto SetWindowPtr(HWND hWnd, int index, auto new_value) -> decltype(new_value)
{
	SetLastError(0);
	LONG_PTR previous_value = SetWindowLongPtr(hWnd, index, reinterpret_cast<LONG_PTR>(new_value));
	SHOUJIN_ASSERT_WIN32(previous_value || !GetLastError());
	return reinterpret_cast<decltype(new_value)>(previous_value);
}

export namespace shoujin::gui2 {

enum class WindowCreateMode {
	Default,
	CenterParent,
	FillParent
};

struct WindowCreateInfo {
	Size clientSize;
	WindowCreateMode createMode{};
	Point position;
	WindowStyle style{};
	String text;
	Size windowSize;
};

class Window {
public:
	static const bool eventHandled;
	static const bool eventUnhandled;

	struct WindowMessage {
		UINT msgCode;
		WPARAM wParam;
		LPARAM lParam;
	};

	struct MessageResult {
		bool handled;
		LRESULT ret_code;
		MessageResult();
		MessageResult(bool handled, LRESULT ret_code = {});
		operator bool() const;
	};

	struct StyleEventArgs {

	};

	Window(WindowCreateInfo const& = {});
	Window(Window const&);
	Window& operator=(Window const&);
	Window(Window&&) = default;
	Window& operator=(Window&&) = default;
	virtual ~Window();

	[[nodiscard]] bool created() const { return _handle; }
	[[nodiscard]] HWND handle() const { return _handle; }
	[[nodiscard]] Point position() const { return _position; };
	[[nodiscard]] Size clientSize() const { return _clientSize; };
	[[nodiscard]] Size windowSize() const { return _windowSize; };
	[[nodiscard]] WindowStyle style() const { return _style; };
	[[nodiscard]] StringView text() const { return _text; };

	Event<bool, Window*, WindowMessage const&> onWndProcEvent;
	Event<bool, Window*, WindowStyle const&> onStyleChangeEvent;
	Event<bool, Window*> onCloseEvent;
	Event<void, Window*> onDestroyEvent;

	virtual bool onClose();
	virtual bool onStyleChange();
	virtual void onDestroy();

	bool processMessageQueue();
	void close();
	void destroy();
	void show();
	void showModal();

protected:
	virtual bool onWndProc(WindowMessage const& message);

private:
	HWND _handle{};
	Point _position;
	Size _clientSize;
	Size _windowSize;
	WindowStyle _style;
	String _text;

	void createWindow();
	void createWin32Window();
	MessageResult raiseOnStyleChange(WindowMessage const& message);
	MessageResult raiseOnClose();
	MessageResult raiseOnDestroy();
	MessageResult raiseOnWndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	bool readMessage(MSG& msg);
	bool readMessageAsync(MSG& msg);
	void translateAndDispatchMessage(MSG const& msg);

	static LRESULT CALLBACK wndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

const bool Window::eventHandled = true;
const bool Window::eventUnhandled = false;

Window::MessageResult::MessageResult() :
	handled{}, ret_code{} {}

Window::MessageResult::MessageResult(bool handled, LRESULT ret_code) :
	handled{handled}, ret_code{ret_code} {}

Window::MessageResult::operator bool() const { return handled; }

Window::Window(WindowCreateInfo const& createInfo)
{
	_text = createInfo.text;
	_style = createInfo.style == WindowStyle::None ? layout::getDefaultStyle() : createInfo.style;
	auto [style, exstyle] = styleToNative(_style);

	if(createInfo.windowSize) {
		_clientSize = layout::getClientSizeFromWindowSize(createInfo.windowSize, style, exstyle);
		_windowSize = createInfo.windowSize;
	} else {
		_clientSize = createInfo.clientSize ? createInfo.clientSize : layout::getDefaultClientSize();
		_windowSize = layout::getWindowSizeFromClientSize(_clientSize, style, exstyle);
	}

	_position = layout::getCenteredPosition(_windowSize);
}

Window::Window(Window const& oth) :
	_handle{},
	_position{oth._position},
	_clientSize{oth._clientSize},
	_windowSize{oth._windowSize},
	_style{oth._style},
	_text{oth._text}
{
}

Window& Window::operator=(Window const& rhs)
{
	if(this == &rhs)
		return *this;

	destroy();

	_handle = {};
	_position = rhs._position;
	_clientSize = rhs._clientSize;
	_windowSize = rhs._windowSize;
	_style = rhs._style;
	_text = rhs._text;

	return *this;
}

Window::~Window()
{
	destroy();
}

bool Window::onClose()
{
	return eventUnhandled;
}

bool Window::onStyleChange()
{
	return eventUnhandled;
}

void Window::onDestroy()
{
}

bool Window::processMessageQueue()
{
	if(!created())
		return false;

	MSG msg;
	while(created() && readMessageAsync(msg))
		translateAndDispatchMessage(msg);

	return created();
}

void Window::close()
{
	if(created())
		SendMessage(_handle, WM_CLOSE, 0, 0);
}

void Window::destroy()
{
	if(created())
		win32api::destroyWindow(_handle);
}

void Window::show()
{
	if(!created()) {
		this->createWindow();
		//RaiseOnInitialize();
		//RaiseOnReady();
	}

	win32api::showWindow(_handle, SW_SHOW);
	win32api::updateWindow(_handle);
	processMessageQueue();
}

void Window::showModal()
{
	show();

	MSG msg;
	while(_handle && readMessage(msg))
		translateAndDispatchMessage(msg);
}

bool Window::onWndProc(WindowMessage const& message)
{
	switch(message.msgCode) {
		//case WM_CREATE:
		//	return RaiseOnCreate(message);
		case WM_CLOSE:
			return raiseOnClose();
			//	case WM_COMMAND:
			//		return RaiseOnCommand(message);
			//	case WM_PAINT:
			//		return RaiseOnPaint();
			//	case WM_SIZING:
			//		return RaiseOnSizing(message);
			//	case WM_EXITSIZEMOVE:
			//		return RaiseOnSizingFinished();
			//	case WM_KEYDOWN:
			//		return RaiseOnKeyDown(message);
			//	case WM_KEYUP:
			//		return RaiseOnKeyUp(message);
			//	case WM_CHAR:
			//		return RaiseOnKeyPress(message);
			//	case WM_LBUTTONDOWN:
			//	case WM_RBUTTONDOWN:
			//	case WM_MBUTTONDOWN:
			//		return RaiseOnMouseDown(message);
			//	case WM_LBUTTONUP:
			//	case WM_RBUTTONUP:
			//	case WM_MBUTTONUP:
			//		return RaiseOnMouseUp(message);
			//	case WM_MOUSEMOVE:
			//		return RaiseOnMouseMove(message);
		case WM_DESTROY:
			return raiseOnDestroy();
		case WM_STYLECHANGING:
			return raiseOnStyleChange(message);
	}

	return eventUnhandled;
}

void Window::createWindow()
{
	createWin32Window();
}

void Window::createWin32Window()
{
	HINSTANCE hinstance = win32api::getModuleHandle(NULL);
	WNDCLASSEX wc;

	auto className = TEXT("SHOUJIN");

	if(!win32api::getClassInfoEx(hinstance, className, &wc)) {
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = wndProcStatic;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = className;
		wc.hIconSm = NULL;

		win32api::registerClassEx(&wc);
	}

	auto [style, exstyle] = styleToNative(_style);

	HWND hWnd =
		win32api::createWindowEx(
			0,
			className,
			_text.c_str(),
			style,
			_position.x,
			_position.y,
			_windowSize.x,
			_windowSize.y,
			/*hWnd_parent*/ NULL,
			nullptr,
			hinstance,
			this);

	SHOUJIN_ASSERT(hWnd);
	SHOUJIN_ASSERT((L"_handle not created. If creating a Comctl32 control, set subclass_window to true.", created()));
}

Window::MessageResult Window::raiseOnStyleChange(WindowMessage const& message)
{
	STYLESTRUCT ss; 
	auto result = onStyleChange();
	return result | (onCloseEvent ? onCloseEvent(this) : eventUnhandled);
}

Window::MessageResult Window::raiseOnClose()
{
	auto result = onClose();
	return result | (onCloseEvent ? onCloseEvent(this) : eventUnhandled);
}

Window::MessageResult Window::raiseOnDestroy()
{
	onDestroy();
	onDestroyEvent(this);

	_handle = {};
	//_window_taborder.release();

	return eventHandled;
}

Window::MessageResult Window::raiseOnWndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	WindowMessage wmsg{msg, wParam, lParam};
	auto result = onWndProc(wmsg);
	return result | (onWndProcEvent ? onWndProcEvent(this, wmsg) : eventUnhandled);
}

bool Window::readMessage(MSG& msg)
{
	return win32api::getMessage(&msg, _handle, 0, 0);
}

bool Window::readMessageAsync(MSG& msg)
{
	return win32api::peekMessage(&msg, _handle, 0, 0, PM_REMOVE);
}

void Window::translateAndDispatchMessage(MSG const& msg)
{
	win32api::translateMessage(&msg);
	//if(!RaiseOnDispatchMessage(msg))
	win32api::dispatchMessage(&msg);
}

LRESULT CALLBACK Window::wndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* self;

	if(msg == WM_NCCREATE) {
		CREATESTRUCT& createparam = *reinterpret_cast<LPCREATESTRUCT>(lParam);
		self = SHOUJIN_ASSERT(reinterpret_cast<Window*>(createparam.lpCreateParams));
		self->_handle = hWnd;

		SetWindowPtr(hWnd, GWLP_USERDATA, self);
	} else
		self = GetWindowPtr<Window*>(hWnd, GWLP_USERDATA);

	if(self) {
		auto message_result = self->raiseOnWndProc(msg, wParam, lParam);
		return message_result ? message_result.ret_code : DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

}
