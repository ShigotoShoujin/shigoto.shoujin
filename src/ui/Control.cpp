#include "Control.hpp"
#include "ControlGroup.hpp"

Control& Control::_move(Control& other) noexcept
{
	control_group = other.control_group;
	control_id = other.control_id;
	taborder = other.taborder;
	tabstop = other.tabstop;

	other.control_group = {};
	other.control_id = {};
	other.taborder = {};
	other.tabstop = {};

	return *this;
}

Control::Control(Control&& other) noexcept :
	Window{std::move(other)}
{
	_move(other);
}

Control& Control::operator=(Control&& other) noexcept
{
	if(this == &other)
		return *this;

	this->~Control();
	Window::operator=(std::move(other));
	return _move(other);
}

Control::Control(const Window::WindowCreateInfo& wci) noexcept :
	Window{wci}, control_group{new ControlGroup(this)}
{}

Control::~Control() noexcept
{
	if(control_group) {
		delete control_group;
		control_group = nullptr;
	}
}

void Control::SetFocus() noexcept
{
	::SetFocus(hwnd);
}

void Control::AddChild(Control&& control) noexcept
{
	control_group->AddControl(std::move(control));
}

bool Control::BeforeKeyDown(HWND hwnd, WPARAM wparam) noexcept
{
	switch(wparam) {
		case VK_TAB: {
			bool cycle_up = GetAsyncKeyState(VK_SHIFT);
			control_group->CycleTab(cycle_up);
			return true;
		}
		case VK_ESCAPE:
			Destroy();
			return true;
	}

	return Window::BeforeKeyDown(hwnd, wparam);
}

void Control::SetParent(const Window& parent_window) noexcept
{
	SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CHILD);
	::SetParent(hwnd, parent_window.GetHandle());
	SetWindowPos(hwnd, 0, 0, 0, window_size.cx, window_size.cy, SWP_NOZORDER | SWP_NOMOVE);
	Show();
}
