#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "window_layout.hpp"

namespace shoujin::gui {

class Window : public WindowLayout {
	HWND _hwnd;
	HWND _hparentwnd;

public:
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) noexcept;
	Window& operator=(Window&&) noexcept;

	Window();
	Window(const WindowLayout& layout, HWND hparentwnd = nullptr);
	virtual ~Window();

	[[nodiscard]] HWND hwnd() const { return _hwnd; }

	bool ProcessMessages();
	void Show();
	void ShowModal();

protected:
	struct WindowMessage {
		UINT msg;
		WPARAM wparam;
		LPARAM lparam;
	};

	virtual bool OnDispatchMessage(MSG& msg);
	virtual bool OnWndProc(const WindowMessage& message);
	void ProcessOnPaintMessageFromDC(HDC hsourcedc);

private:
	void CreateHandle();
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void ProcessMessage(MSG msg);
};

}
