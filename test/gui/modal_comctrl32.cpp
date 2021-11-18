#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include <Windows.h>

#include "../../src/assert/assert.hpp"
#include "../../src/gui/logging/message_logger.hpp"

using namespace shoujin;
using namespace shoujin::gui;

LRESULT CALLBACK WndProcSubClass(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	logging::LogWndProcMessage(hwnd, msg, wparam, lparam);

	LONG_PTR swlp_result = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	WNDPROC default_proc = reinterpret_cast<WNDPROC>(swlp_result);

	switch(msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	};

	return CallWindowProc(default_proc, hwnd, msg, wparam, lparam);
}

TEST_CLASS(ModalComctrl32Test) {
public:
	TEST_METHOD_INITIALIZE(TestInitialize)
	{
		shoujin::gui::logging::_activate_wndproc_messagelog_ = true;
	}

	TEST_METHOD_CLEANUP(TestCleanup)
	{
		shoujin::gui::logging::_activate_wndproc_messagelog_ = false;
	}

	TEST_METHOD(ShowModalTextBox) {
		HWND hwnd = CreateWindowEx(0, L"EDIT", L"Hello", WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE | ES_MULTILINE, 780, 310, 1000, 720, 0, 0, GetModuleHandle(0), 0);

		LONG_PTR default_proc = SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProcSubClass));
		LONG_PTR result = SetWindowLongPtr(hwnd, GWLP_USERDATA, default_proc);

		//Remove this to view the window
		PostMessage(hwnd, WM_CLOSE, 0, 0);

		MSG msg;
		while(GetMessage(&msg, 0, 0, 0) && msg.message != WM_DESTROY) {
			if(msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
};
