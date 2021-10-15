#include "CppUnitTest.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <ui/Window.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(UITests) {
	SIZE screen{};

public:
	TEST_METHOD_INITIALIZE(TestInitialize)
	{
		screen.cx = GetSystemMetrics(SM_CXSCREEN);
		screen.cy = GetSystemMetrics(SM_CYSCREEN);
	}

	TEST_METHOD(WindowTestCustomLayout) {
		Window w({.text = TEXT("WindowTestCustomLayout"), .position{screen.cx / 5, screen.cy / 5}});
		w.Show();
		w.MessageLoop();
	}

	TEST_METHOD(WindowTestCenterDesktop) {
		Window w({.text = TEXT("WindowTestCenterDesktop"), .layout = Window::Layout::CenterParent});
		w.Show();
		w.MessageLoop();
	}

	TEST_METHOD(WindowTestFillDesktop) {
		Window w({.text = TEXT("WindowTestFillDesktop"), .layout = Window::Layout::FillParent});
		w.Show();
		w.MessageLoop();
	}

	TEST_METHOD(WindowTestChildWindows) {
		Window::WindowCreateInfo wci_root{
			.text = TEXT("WindowTestChildWindows"),
			.layout = Window::Layout::CenterParent,
			.client_size{screen.cx / 3, screen.cy / 3},
			.style{WS_VISIBLE | Window::DEFAULT_STYLE}};

		Window root_wnd(wci_root);

		Window::WindowCreateInfo wci_fill{
			.hwnd_parent = root_wnd.GetHandle(),
			.text = TEXT("fill"),
			.layout = Window::Layout::FillParent,
			.style{WS_VISIBLE},
			.ex_style = WS_EX_CLIENTEDGE};

		Window fill_wnd(wci_fill);

		Window::WindowCreateInfo wci_center{
			.hwnd_parent = fill_wnd.GetHandle(),
			.text = TEXT("fill"),
			.layout = Window::Layout::CenterParent,
			.window_size = {screen.cx / 5, screen.cy / 5},
			.style{WS_VISIBLE},
			.ex_style = WS_EX_CLIENTEDGE};

		Window center_wnd(wci_center);
		root_wnd.Show();

		while(root_wnd.MessageUpdate()) {
			fill_wnd.MessageUpdate();
			center_wnd.MessageUpdate();
		}
	}
};
