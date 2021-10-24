#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <map>

class Control;

class ControlGroup {
	friend Control;
	std::map<HWND, Control*> control_map;
	std::multimap<int, Control*> tab_map;

	ControlGroup(const ControlGroup&) noexcept = delete;
	ControlGroup& operator=(const ControlGroup&) noexcept = delete;
	ControlGroup(ControlGroup&&) noexcept = delete;
	ControlGroup& operator=(ControlGroup&&) noexcept = delete;

	ControlGroup() noexcept;
	~ControlGroup() noexcept;

	Control* AddControl(Control&& control);
	void CycleTab(bool cycle_up) noexcept;

	int GetMaxTabOrder() const noexcept;
	Control* FindControlByHandle(HWND control) noexcept;
	Control* FindNextControlInTabOrder(Control* control, bool cycle_up) noexcept;
	void SetFocusToControl(Control* control) noexcept;
	void SetFocusToFirstControl() noexcept;
};
