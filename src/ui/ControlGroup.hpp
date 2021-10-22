#pragma once
#include "Control.hpp"
#include <map>

class ControlGroup {
	const Control* parent_control;
	std::map<HWND, Control> control_map;
	std::multimap<int, Control*> tab_map;

public:
	ControlGroup(const ControlGroup&) noexcept = delete;
	ControlGroup& operator=(const ControlGroup&) noexcept = delete;
	ControlGroup(ControlGroup&&) noexcept = delete;
	ControlGroup& operator=(ControlGroup&&) noexcept = delete;

	ControlGroup(const Control* parent_control) noexcept;
	~ControlGroup() noexcept;

	Control* AddControl(Control&& control) noexcept;
	void CycleTab(bool cycle_up) noexcept;

private:
	int GetMaxTabOrder() const noexcept;
	Control* FindControlByHandle(HWND control) noexcept;
	Control* FindNextControlInTabOrder(Control* control, bool cycle_up) noexcept;
	void SetFocusToControl(Control* control) noexcept;
	void SetFocusToFirstControl() noexcept;
};
