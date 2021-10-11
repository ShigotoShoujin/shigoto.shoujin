#include "DialogWindow.hpp"

DialogWindow::DialogWindow(Window::Args args) noexcept :
	Window{args} {}

DialogWindow::~DialogWindow()
{
	for(auto& c : control_map)
		delete c.second;
}

bool DialogWindow::BeforeKeyDown(HWND hwnd, WPARAM wparam) noexcept
{
	switch(wparam) {
		case VK_TAB: {
			bool up = GetAsyncKeyState(VK_SHIFT) & 0x8000;
			CycleTab(up);
			return true;
		}
		case VK_ESCAPE:
			Destroy();
			return true;
	}

	return Window::BeforeKeyDown(hwnd, wparam);
}

void DialogWindow::AddOwnedControl(Control* control) noexcept
{
	if(control->IsAutoTabOrder())
		control->SetTabOrder(GetMaxTabOrder() + 1);

	control_map.emplace(control->GetHandle(), control);
	tab_map.emplace(control->GetTabOrder(), control);
}

int DialogWindow::GetMaxTabOrder() const noexcept
{
	auto it = tab_map.crbegin();
	if(it != tab_map.crend())
		return it->first;
	return 0;
}

Control* DialogWindow::FindControlByHandle(HWND control) const noexcept
{
	auto it = control_map.find(control);
	if(it == control_map.end())
		return nullptr;
	return it->second;
}

Control* DialogWindow::FindNextControlInTabOrder(Control* control, bool up) const noexcept
{
	if(tab_map.size() == 1)
		return control;

	auto current = tab_map.find(control->GetTabOrder());
	if(current == tab_map.end())
		return nullptr;

	if(up) {
		if(current == tab_map.begin())
			return tab_map.rbegin()->second;

		return std::prev(current)->second;
	}

	auto next = std::next(current);
	if(next == tab_map.end())
		return tab_map.begin()->second;

	return next->second;
}

void DialogWindow::SetFocusToControl(Control* control) noexcept
{
	control->SetFocus();
}

void DialogWindow::SetFocusToFirstControl() noexcept
{
	const auto it = tab_map.cbegin();
	if(it != tab_map.cend())
		SetFocusToControl(it->second);
}

void DialogWindow::CycleTab(bool up) noexcept
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

	control = FindNextControlInTabOrder(control, up);
	if(control)
		control->SetFocus();
}
