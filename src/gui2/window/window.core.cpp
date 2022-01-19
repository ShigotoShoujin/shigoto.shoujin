module;
#include "../../assert/assert.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Window : Core;
import : Layout;
import Shoujin.Event;
import Shoujin.Gui.Types;
import Shoujin.Gui.Win32Api;

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

enum class WindowStyle : int {
	Border
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

	Window() = default;
	virtual ~Window() = default;

	[[nodiscard]] bool created() const { return _handle; }
	[[nodiscard]] HWND handle() const { return _handle; }
	[[nodiscard]] Point position() const { return _position; };
	[[nodiscard]] Size clientSize() const { return _clientSize; };
	[[nodiscard]] Size windowSize() const { return _windowSize; };

	Event<bool> onCloseEvent;
	Event<bool, WindowMessage const&> onWndProcEvent;
	Event<> onDestroyEvent;

	virtual bool onClose();
	virtual void onDestroy();

	bool processMessageQueue();
	void show();
	void showModal();

protected:
	virtual bool onWndProc(WindowMessage const& message);

private:
	HWND _handle{};
	Point _position;
	Size _clientSize;
	Size _windowSize;

	void createWindow();
	void createWin32Window();
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

bool Window::onClose()
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
	while(_handle && readMessageAsync(msg))
		translateAndDispatchMessage(msg);

	return created();
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

	//ProcessMessageQueue();
}

void Window::showModal()
{
	show();

	MSG msg;
	while(_handle && readMessage(msg))
		translateAndDispatchMessage(msg);
}

//void Window::showModal()
//{
//	Show();
//
//	MSG msg;
//	while(_handle && ReadMessage(msg))
//		TranslateAndDispatchMessage(msg);
//}

bool Window::onWndProc(WindowMessage const& message)
{
	switch(message.msgCode) {
			//	case WM_CREATE:
			//		return RaiseOnCreate(message);
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

	DWORD style = WS_OVERLAPPEDWINDOW;

	Size screen = layout::getScreenSize();

	_clientSize = {
		screen.x / 3,
		screen.y / 3};

	RECT rect{0, 0, _clientSize.x, _clientSize.y};
	win32api::adjustWindowRectEx(&rect, style, NULL, 0);
	_windowSize = {rect.right - rect.left, rect.bottom - rect.top};
	_position = (screen - _windowSize) / 2;

	HWND hWnd =
		win32api::createWindowEx(
			0,
			className,
			className,
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

Window::MessageResult Window::raiseOnClose()
{
	auto result = onClose();
	return result | (onCloseEvent ? onCloseEvent() : eventUnhandled);
}

Window::MessageResult Window::raiseOnDestroy()
{
	onDestroy();
	onDestroyEvent();

	_handle = NULL;
	//_window_taborder.release();

	return eventHandled;
}

Window::MessageResult Window::raiseOnWndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	WindowMessage wmsg{msg, wParam, lParam};
	auto result = onWndProc(wmsg);
	return result | (onWndProcEvent ? onWndProcEvent(wmsg) : eventUnhandled);
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
