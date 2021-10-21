#pragma once
#include "Control.hpp"
#include <map>

class ControlGroup {
	const Control* parent_control;
	std::map<HWND, Control> control_map;
	std::multimap<int, Control*> tab_map;

public:
	DISABLE_COPY_AND_MOVE(ControlGroup)

	ControlGroup(const Control* parent_control) noexcept;
	~ControlGroup() noexcept;

	//[[nodiscard]] inline ControlGroup* begin() noexcept { auto a = control_map.emplace};
	//[[nodiscard]] inline ControlGroup* end() noexcept { return control_group->end(); };

	Control* AddControl(Control&& control) noexcept;
	void CycleTab(bool cycle_up) noexcept;

private:
	int GetMaxTabOrder() const noexcept;
	Control* FindControlByHandle(HWND control) noexcept;
	Control* FindNextControlInTabOrder(Control* control, bool cycle_up) noexcept;
	void SetFocusToControl(Control* control) noexcept;
	void SetFocusToFirstControl() noexcept;
};
