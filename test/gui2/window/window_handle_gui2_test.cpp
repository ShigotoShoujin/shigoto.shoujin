import Shoujin.Gui.Window;
import Shoujin.Win32Api;

#include "../cppunittest_util.hpp"
#include "CppUnitTest.h"
#include <utility>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace shoujin::gui2;

#define SHOUJIN_TEST_GUI_VISIBLE

#ifdef SHOUJIN_TEST_GUI_VISIBLE
constexpr DWORD style = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
#else
constexpr DWORD style = WS_OVERLAPPEDWINDOW;
#endif

#define WINDOW_HANDLE_CREATE_PARAM TEXT("Shoujin"), TEXT("Shoujin WindowHandle"), CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, style, 0, nullptr

namespace shoujin::test::gui2 {
int onCreateHandleCount{};
int onDestroyHandleCount{};

class WindowHandleStub : public WindowHandle {
public:
	WindowHandleStub(LPCTSTR className, LPCTSTR windowName, int positionX, int positionY, int windowSizeX, int windowSizeY, DWORD style, DWORD exstyle, WindowHandle* parent) :
		WindowHandle(className, windowName, positionX, positionY, windowSizeX, windowSizeY, style, exstyle, parent) {}

	WindowHandleStub(WindowHandleStub const& oth) :
		WindowHandle(oth), hidden(oth.hidden) {}

	WindowHandleStub& operator=(WindowHandleStub const& rhs)
	{
		WindowHandle::operator=(rhs);
		hidden = rhs.hidden;
		return *this;
	}

	WindowHandleStub(WindowHandleStub&& oth) noexcept :
		WindowHandle(std::move(oth)), hidden(oth.hidden) {}

	WindowHandleStub& operator=(WindowHandleStub&& rhs) noexcept
	{
		WindowHandle::operator=(std::move(rhs));
		hidden = rhs.hidden;
		return *this;
	}

	virtual ~WindowHandleStub() = default;

	static bool continueUntilCallback(void* userdata)
	{
		return !reinterpret_cast<WindowHandleStub*>(userdata)->hidden;
	}

	void modalMessageLoop()
	{
		if(hidden) {
			hidden = false;
			shoujin::win32api::showWindow(getHandle(), SW_SHOW);
		}

		WindowHandle::modalMessageLoopUntil({continueUntilCallback, this});
	}

private:
	bool hidden{};

	void onCreateHandle() override
	{
		++onCreateHandleCount;
	}

	void onDestroyHandle() override
	{
		++onDestroyHandleCount;
	}

	bool onHandleProc(Message const& message) override
	{
		if(message.msg == WM_CLOSE) {
			shoujin::win32api::showWindow(getHandle(), SW_HIDE);
			hidden = true;
			return eventHandled;
		}

		return WindowHandle::onHandleProc(message);
	}
};

TEST_CLASS(WindowHandleTest) {
	void showModalIfGuiVisible(WindowHandleStub & handle)
	{
#ifdef SHOUJIN_TEST_GUI_VISIBLE
		handle.modalMessageLoop();
#endif
	}

public:
	TEST_METHOD(WindowHandle_CopyAssignment_DifferentHandles) {
		WindowHandleStub wnd1(WINDOW_HANDLE_CREATE_PARAM);
		showModalIfGuiVisible(wnd1);

		WindowHandleStub wnd2(WINDOW_HANDLE_CREATE_PARAM);
		showModalIfGuiVisible(wnd2);

		auto prevHandle = wnd1.getHandle();
		wnd1 = wnd2;

		showModalIfGuiVisible(wnd1);

		Assert::AreNotEqual(prevHandle, wnd1.getHandle());
		Assert::AreNotEqual(wnd1.getHandle(), wnd2.getHandle());
	}

	TEST_METHOD(WindowHandle_CopyConstructor_DifferentHandles) {
		WindowHandleStub wnd1(WINDOW_HANDLE_CREATE_PARAM);
		showModalIfGuiVisible(wnd1);

		WindowHandleStub wnd2(wnd1);
		showModalIfGuiVisible(wnd2);

		Assert::AreNotEqual(wnd1.getHandle(), wnd2.getHandle());
	}

	TEST_METHOD(WindowHandle_ModalMessageLoopUntil_LoopUntilFalse) {
		WindowHandleStub wnd(WINDOW_HANDLE_CREATE_PARAM);
		int count{};

		wnd.modalMessageLoopUntil({[](void* ud) { return 3 != ++(*reinterpret_cast<int*>(ud)); }, &count});

		Assert::AreEqual(3, count);
	}

	TEST_METHOD(WindowHandle_MoveAssignment_SameHandles) {
		WindowHandleStub wnd1(WINDOW_HANDLE_CREATE_PARAM);
		showModalIfGuiVisible(wnd1);

		WindowHandleStub wnd2(WINDOW_HANDLE_CREATE_PARAM);
		showModalIfGuiVisible(wnd2);

		auto prevHandle = wnd2.getHandle();
		wnd1 = std::move(wnd2);

		showModalIfGuiVisible(wnd1);

		Assert::IsTrue(!wnd2.getHandle());
		Assert::AreEqual(prevHandle, wnd1.getHandle());
	}

	TEST_METHOD(WindowHandle_MoveConstructor_SameHandles) {
		WindowHandleStub wnd1(WINDOW_HANDLE_CREATE_PARAM);
		showModalIfGuiVisible(wnd1);

		auto prevHandle = wnd1.getHandle();
		WindowHandleStub wnd2(std::move(wnd1));
		showModalIfGuiVisible(wnd2);

		Assert::IsTrue(!wnd1.getHandle());
		Assert::AreEqual(prevHandle, wnd2.getHandle());
	}

	TEST_METHOD(WindowHandle_ObjectGoesOutOfScope_Destroyed) {
		HWND hwnd;

		{
			WindowHandleStub wnd(WINDOW_HANDLE_CREATE_PARAM);
			hwnd = wnd.getHandle();
		}

		Assert::IsFalse(IsWindow(hwnd));
	}

	TEST_METHOD(WindowHandle_OnCreate_CallOnCreateHandle) {
		WindowHandleStub wnd(WINDOW_HANDLE_CREATE_PARAM);
		Assert::AreEqual(1, onCreateHandleCount);
	}

	TEST_METHOD(WindowHandle_OnDestroy_CallOnDestroyHandle) {
		{
			WindowHandleStub wnd(WINDOW_HANDLE_CREATE_PARAM);
			showModalIfGuiVisible(wnd);
		}
		Assert::AreEqual(1, onDestroyHandleCount);
	}
};

}
