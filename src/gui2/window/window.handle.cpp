module;
#include "../../assert/assert.hpp"
#include "../../event.hpp"
#include <utility>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Window : WindowHandle;
import Shoujin.Win32Api;

using namespace shoujin;

export namespace shoujin::gui2 {

class WindowHandle {
public:
	static const bool eventHandled;
	static const bool eventUnhandled;

	struct Message {
		UINT msg;
		WPARAM wParam;
		LPARAM lParam;
	};

	struct MessageResult {
		bool handled;
		LRESULT returnCode;

		MessageResult() :
			handled{}, returnCode{} {}

		MessageResult(bool handled, LRESULT returnCode = {}) :
			handled{handled}, returnCode{returnCode} {}

		operator bool() const { return handled; }
	};

	WindowHandle(LPCTSTR className, LPCTSTR windowName, int positionX, int positionY, int windowSizeX, int windowSizeY, DWORD style, DWORD exstyle, WindowHandle* parent);
	WindowHandle(WindowHandle const&);
	WindowHandle& operator=(WindowHandle const&);
	WindowHandle(WindowHandle&&) noexcept;
	WindowHandle& operator=(WindowHandle&&) noexcept;
	virtual ~WindowHandle();
	friend void swap(WindowHandle&, WindowHandle&) noexcept;

	[[nodiscard]] HWND getHandle() const noexcept { return handle; }
	[[nodiscard]] WindowHandle* getParent() const noexcept { return parent; }

	Event<void, WindowHandle*> CreateHandleEvent;
	Event<void, WindowHandle*> DestroyHandleEvent;
	Event<bool, WindowHandle*, Message const&> handleProcEvent;

	void modalMessageLoop();
	void modalMessageLoopUntil(Event<bool> untilFalse);
	bool processMessageQueue();

protected:
	virtual void onCreateHandle();
	virtual void onDestroyHandle();
	virtual bool onHandleProc(Message const& message);

private:
	HWND handle{};
	WindowHandle* parent{};

	MessageResult raiseOnCreateHandle();
	MessageResult raiseOnDestroyHandle();
	MessageResult raiseOnHandleProc(Message const& message);

	void destroy();
	bool readMessage(MSG& msg);
	bool readMessageAsync(MSG& msg);
	void translateAndDispatchMessage(MSG const& msg);
	static LRESULT CALLBACK windowProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

const bool WindowHandle::eventHandled = true;
const bool WindowHandle::eventUnhandled = false;

WindowHandle::WindowHandle(LPCTSTR className, LPCTSTR windowName, int positionX, int positionY, int windowSizeX, int windowSizeY, DWORD style, DWORD exstyle, WindowHandle* parent) :
	parent{parent}
{
	HINSTANCE hInstance = win32api::getModuleHandle(NULL);
	WNDCLASSEX wc;
	ATOM wcAtom;

	if(!(wcAtom = win32api::getClassInfoEx(hInstance, className, &wc))) {
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = windowProcStatic;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = className;
		wc.hIconSm = NULL;

		wcAtom = win32api::registerClassEx(&wc);
	}

	HWND hWndParent = parent ? parent->handle : nullptr;

	win32api::createWindowEx(
		exstyle,
		reinterpret_cast<LPCTSTR>(wcAtom),
		windowName,
		style,
		positionX,
		positionY,
		windowSizeX,
		windowSizeY,
		hWndParent,
		nullptr,
		hInstance,
		this);

	processMessageQueue();
}

WindowHandle::WindowHandle(WindowHandle const& oth)
{
	if(!oth.handle)
		return;

	constexpr size_t maxLength = 256;
	TCHAR className[maxLength];
	win32api::getClassName(oth.handle, className, maxLength);

	auto textVec = win32api::util::getWindowTextVector(oth.handle);
	LPCTSTR windowName = textVec.data();

	RECT rect;
	win32api::getWindowRect(oth.handle, &rect);
	int x = rect.left;
	int y = rect.top;
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	DWORD style = win32api::util::getWindowPtr<DWORD>(oth.handle, GWL_STYLE);
	DWORD exstyle = win32api::util::getWindowPtr<DWORD>(oth.handle, GWL_EXSTYLE);

	WindowHandle local(className, windowName, x, y, w, h, style, exstyle, oth.parent);
	swap(*this, local);
}

WindowHandle& WindowHandle::operator=(WindowHandle const& rhs)
{
	WindowHandle local(rhs);
	swap(*this, local);
	return *this;
}

WindowHandle::WindowHandle(WindowHandle&& oth) noexcept
{
	swap(*this, oth);
}

WindowHandle& WindowHandle::operator=(WindowHandle&& rhs) noexcept
{
	WindowHandle local(std::move(rhs));
	swap(*this, local);
	return *this;
}

WindowHandle::~WindowHandle()
{
	destroy();
}

void swap(WindowHandle& a, WindowHandle& b) noexcept
{
	SHOUJIN_ASSERT(&a != &b);

	// TODO Find why the below line create a cluster of compiler errors
	// using std::swap;
	// swap(first.handle, second.handle);
	// swap(first.parent, second.parent);*/

	if(a.handle)
		win32api::util::setWindowPtr(a.handle, GWLP_USERDATA, &b);

	if(b.handle)
		win32api::util::setWindowPtr(b.handle, GWLP_USERDATA, &a);

	std::swap(a.handle, b.handle);
	std::swap(a.parent, b.parent);
}

void WindowHandle::modalMessageLoop()
{
	MSG msg;
	while(handle && readMessage(msg))
		translateAndDispatchMessage(msg);
}

void WindowHandle::modalMessageLoopUntil(Event<bool> untilFalse)
{
	MSG msg;
	while(handle && untilFalse() && readMessage(msg))
		translateAndDispatchMessage(msg);
}

bool WindowHandle::processMessageQueue()
{
	MSG msg;
	while(handle && readMessageAsync(msg))
		translateAndDispatchMessage(msg);

	return handle;
}

void WindowHandle::onCreateHandle()
{
}

void WindowHandle::onDestroyHandle()
{
}

bool WindowHandle::onHandleProc(Message const& message)
{
	return eventUnhandled;
}

WindowHandle::MessageResult WindowHandle::raiseOnCreateHandle()
{
	onCreateHandle();
	CreateHandleEvent(this);
	return eventUnhandled;
}

WindowHandle::MessageResult WindowHandle::raiseOnDestroyHandle()
{
	onDestroyHandle();
	DestroyHandleEvent(this);

	handle = {};
	parent = {};

	return eventHandled;
}

WindowHandle::MessageResult WindowHandle::raiseOnHandleProc(Message const& message)
{
	auto result = onHandleProc(message);
	result |= (handleProcEvent ? handleProcEvent(this, message) : eventUnhandled);

	switch(message.msg) {
		case WM_NCCREATE:
			raiseOnCreateHandle();
			break;
		case WM_NCDESTROY:
			raiseOnDestroyHandle();
			break;
	}

	return result;
}

void WindowHandle::destroy()
{
	if(handle)
		shoujin::win32api::destroyWindow(handle);
}

bool WindowHandle::readMessage(MSG& msg)
{
	return win32api::getMessage(&msg, handle, 0, 0);
}

bool WindowHandle::readMessageAsync(MSG& msg)
{
	return win32api::peekMessage(&msg, handle, 0, 0, PM_REMOVE);
}

void WindowHandle::translateAndDispatchMessage(MSG const& msg)
{
	win32api::translateMessage(&msg);
	win32api::dispatchMessage(&msg);
}

LRESULT CALLBACK WindowHandle::windowProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WindowHandle* self;

	if(msg == WM_NCCREATE) {
		CREATESTRUCT& createparam = *reinterpret_cast<LPCREATESTRUCT>(lParam);
		self = SHOUJIN_ASSERT(reinterpret_cast<WindowHandle*>(createparam.lpCreateParams));
		self->handle = hWnd;

		win32api::util::setWindowPtr(hWnd, GWLP_USERDATA, self);
	} else
		self = win32api::util::getWindowPtr<WindowHandle*>(hWnd, GWLP_USERDATA);

	if(self) {
		auto messageResult = self->raiseOnHandleProc({msg, wParam, lParam});
		return messageResult ? messageResult.returnCode : DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
}
