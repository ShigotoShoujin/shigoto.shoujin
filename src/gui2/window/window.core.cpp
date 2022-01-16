module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Window : Core;
import Shoujin.Gui.Win32Api;

export namespace shoujin::gui2 {

enum class WindowStyle : int {
	Border
};

class Window {
public:
	Window() = default;
	virtual ~Window() = default;

	[[nodiscard]] bool created() const noexcept { return _handle; }
	[[nodiscard]] HWND handle() const noexcept { return _handle; }

	void Show();
	//void ShowModal();

private:
	HWND _handle{};
};

void Window::Show()
{
	//if(!created()) {
	//	CreateHandle();
	//	//RaiseOnInitialize();
	//	//RaiseOnReady();
	//}

	//ShowWindow(*_handle, SW_SHOW);
	win32api::UpdateWindow(_handle);

	//ProcessMessageQueue();
}

//void Window::ShowModal()
//{
//	Show();
//
//	MSG msg;
//	while(_handle && ReadMessage(msg))
//		TranslateAndDispatchMessage(msg);
//}

}
