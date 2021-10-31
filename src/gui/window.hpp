#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {
class Window {
public:
	Window(Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) noexcept;
	Window& operator=(Window&&) noexcept;

	Window();
	virtual ~Window();

	[[nodiscard]] HWND inline GetHandle() const { return _hwnd; }

	bool ProcessMessages();
	void Show();
	void ShowModal();

protected:
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) noexcept;

private:
	void WIP_Create();
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
	HWND _hwnd;
};
}
