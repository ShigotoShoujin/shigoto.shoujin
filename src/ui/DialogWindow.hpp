#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Window.hpp"
#include "Control.hpp"
#include <map>

class DialogWindow : public Window {
	std::map<HWND, Control*> control_map;
	std::multimap<int, Control*> tab_map;

public:
	DialogWindow(Window::Args args) noexcept;
	virtual ~DialogWindow();

	virtual bool BeforeKeyDown(HWND hwnd, WPARAM wparam) noexcept override;

	void AddOwnedControl(Control* control) noexcept;

private:
	int GetMaxTabOrder() const noexcept;
	Control* FindControlByHandle(HWND control) const noexcept;
	Control* FindNextControlInTabOrder(Control* control, bool up) const noexcept;
	void SetFocusToControl(Control* control) noexcept;
	void SetFocusToFirstControl() noexcept;
	void CycleTab(bool up) noexcept;
};
