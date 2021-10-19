#pragma once
#include "Control.hpp"
#include <map>

class ControlGroup {
	const Control* parent_control;
	typedef std::map<HWND, Control> ControlMapT;
	typedef std::multimap<int, Control*> TabMapT;

	ControlMapT control_map;
	TabMapT tab_map;

public:
	DISABLE_COPY_AND_MOVE(ControlGroup)

	ControlGroup(const Control* parent_control) noexcept;
	~ControlGroup() noexcept;

	ControlMapT::const_iterator cbegin() const noexcept { return control_map.cbegin(); }
	ControlMapT::const_iterator cend() const noexcept { return control_map.cend(); }
	ControlMapT::iterator begin() noexcept { return control_map.begin(); }
	ControlMapT::iterator end() noexcept { return control_map.end(); }

	void AddControl(Control&& control) noexcept;
	void CycleTab(bool cycle_up) noexcept;

private:
	int GetMaxTabOrder() const noexcept;
	Control* FindControlByHandle(HWND control) noexcept;
	Control* FindNextControlInTabOrder(Control* control, bool cycle_up) noexcept;
	void SetFocusToControl(Control* control) noexcept;
	void SetFocusToFirstControl() noexcept;
};
