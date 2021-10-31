#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "window_layout.hpp"

namespace shoujin::gui {
class Window : public WindowLayout {
	HWND _hwnd;

public:
	Window(Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) noexcept;
	Window& operator=(Window&&) noexcept;

	Window();
	virtual ~Window();

	[[nodiscard]] HWND inline Handle() const { return _hwnd; }

	bool ProcessMessages();
	void Show();
	void ShowModal();

protected:
	virtual bool OnDispatchMessage(MSG& msg);
	virtual LRESULT OnWndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
	void CreateHandle();
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void ProcessMessage(MSG msg);
};
}
