#include "Control.hpp"

Control::Control(Control&& other) noexcept :
	Window{std::move(other)} {}

Control& Control::operator=(Control&& other) noexcept
{
	if(this == &other)
		return *this;

	Window::operator=(std::move(other));
	return *this;
}

Control::Control(const Window::WindowCreateInfo& wci) noexcept :
	Window{wci} {}

Control::~Control() noexcept {}

void Control::AddChild(Control&& control) noexcept
{
	control.SetParent(*this);
	child_map.emplace(++child_map_last_id, std::move(control));
}

void Control::SetParent(const Window& parent_window) noexcept
{
	SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CHILD);
	::SetParent(hwnd, parent_window.GetHandle());
	SetWindowPos(hwnd, 0, 0, 0, window_size.cx, window_size.cy, SWP_NOZORDER | SWP_NOMOVE);
	Show();
}
