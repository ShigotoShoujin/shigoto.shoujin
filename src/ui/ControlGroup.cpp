#include "ControlGroup.hpp"
#include "Control.hpp"

ControlGroup::ControlGroup() noexcept {}

ControlGroup::~ControlGroup() noexcept
{
	for(auto& c : control_map)
		delete c.second;
}

Control* ControlGroup::AddControl(Control&& control)
{
	Control* ctrl = new Control(std::move(control));

	if(ctrl->tabstop)
		ctrl->taborder = GetMaxTabOrder() + 1;

	ctrl->WndProc(0, 0, 0);

	Control* parent_control = static_cast<Control*>(this);
	ctrl->SetParent(*parent_control);

	auto new_insertion = control_map.emplace(ctrl->hwnd, ctrl).second;
	assert(new_insertion);
	tab_map.emplace(ctrl->taborder, ctrl);

	return ctrl;
}

void ControlGroup::CycleTab(bool cycle_up) noexcept
{
	HWND focus = GetFocus();
	if(!focus) {
		SetFocusToFirstControl();
		return;
	}

	auto control = FindControlByHandle(focus);
	if(!control) {
		SetFocusToFirstControl();
		return;
	}

	control = FindNextControlInTabOrder(control, cycle_up);
	if(control)
		control->SetFocus();
}

int ControlGroup::GetMaxTabOrder() const noexcept
{
	auto it = tab_map.crbegin();
	if(it != tab_map.crend())
		return it->first;
	return 0;
}

Control* ControlGroup::FindControlByHandle(HWND control) noexcept
{
	auto it = control_map.find(control);
	if(it == control_map.end())
		return nullptr;

	return it->second;
}

Control* ControlGroup::FindNextControlInTabOrder(Control* control, bool cycle_up) noexcept
{
	if(tab_map.size() == 1)
		return control;

	auto current = tab_map.find(control->taborder);
	if(current == tab_map.end())
		return nullptr;

	if(cycle_up) {
		if(current == tab_map.begin())
			return tab_map.rbegin()->second;

		return std::prev(current)->second;
	}

	auto next = std::next(current);
	if(next == tab_map.end())
		return tab_map.begin()->second;

	return next->second;
}

void ControlGroup::SetFocusToControl(Control* control) noexcept
{
	control->SetFocus();
}

void ControlGroup::SetFocusToFirstControl() noexcept
{
	const auto it = tab_map.cbegin();
	if(it != tab_map.cend())
		SetFocusToControl(it->second);
}
