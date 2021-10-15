#include <ui/Window.hpp>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(WindowTest) {
	SIZE screen{};

public:
	TEST_METHOD_INITIALIZE(TestInitialize)
	{
		screen.cx = GetSystemMetrics(SM_CXSCREEN);
		screen.cy = GetSystemMetrics(SM_CYSCREEN);
	}

	TEST_METHOD(WindowTestCustomLayout) {
		Window w({.position{screen.cx / 5, screen.cy / 5}, .text = TEXT("WindowTestCustomLayout")});
		w.Show();
		w.MessageLoop();
	}

	TEST_METHOD(WindowTestCenterDesktop) {
		Window w({.layout = Window::Layout::CenterParent, .text = TEXT("WindowTestCenterDesktop")});
		w.Show();
		w.MessageLoop();
	}

	TEST_METHOD(WindowTestFillDesktop) {
		Window w({.layout = Window::Layout::FillParent, .text = TEXT("WindowTestFillDesktop")});
		w.Show();
		w.MessageLoop();
	}

	TEST_METHOD(WindowTestChildWindows) {
		Window::WindowCreateInfo wci_root{
			.client_size{screen.cx / 3, screen.cy / 3},
			.layout = Window::Layout::CenterParent,
			.style{WS_VISIBLE | Window::DEFAULT_STYLE},
			.text = TEXT("WindowTestChildWindows")};

		Window root_wnd(wci_root);

		Window::WindowCreateInfo wci_fill{
			.ex_style = WS_EX_CLIENTEDGE,
			.hwnd_parent = root_wnd.GetHandle(),
			.layout = Window::Layout::FillParent,
			.style{WS_VISIBLE},
			.text = TEXT("fill")};

		Window fill_wnd(wci_fill);

		Window::WindowCreateInfo wci_center{
			.ex_style = WS_EX_CLIENTEDGE,
			.hwnd_parent = fill_wnd.GetHandle(),
			.layout = Window::Layout::CenterParent,
			.style{WS_VISIBLE},
			.text = TEXT("fill"),
			.window_size = {screen.cx / 5, screen.cy / 5}};

		Window center_wnd(wci_center);

		while(root_wnd.MessageUpdate()) {
			fill_wnd.MessageUpdate();
			center_wnd.MessageUpdate();
		}
	}
};
