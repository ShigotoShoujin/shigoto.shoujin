#include "Control.hpp"
#include "ControlGroup.hpp"
#include "Debug.hpp"

Control::Control(Control&& other) noexcept :
	Window{std::move(other)},
	control_id{other.control_id},
	taborder{other.taborder},
	tabstop{other.tabstop}
{}

Control::Control(const ControlCreateInfo& cci) noexcept :
	Window{{
		.class_name = cci.class_name,
		.client_size = cci.client_size,
		.ex_style = cci.ex_style,
		.hwnd_parent = cci.hwnd_parent,
		.layout = cci.layout,
		.position = cci.position,
		.style = cci.style,
		.text = cci.text,
		.window_size = cci.window_size,
	}},
	control_id{cci.control_id},
	taborder{cci.taborder},
	tabstop{cci.tabstop}
{}

Control::Control(const UserControlCreateInfo& cci, LPCTSTR class_name, DWORD style, DWORD ex_style) noexcept :
	Window{{
		.class_name = class_name,
		.client_size = cci.client_size,
		.ex_style = ex_style,
		.layout = cci.layout,
		.position = cci.position,
		.style = style,
		.text = cci.text,
		.window_size = cci.window_size,
	}},
	control_id{cci.control_id},
	taborder{cci.taborder},
	tabstop{cci.tabstop}
{}

Control::~Control() noexcept {}

void Control::SetFocus() noexcept
{
	::SetFocus(hwnd);
}

Control* Control::AddChild(Control* control)
{
	return ControlGroup::AddControl(control);
}

void Control::DrawTabOrder() noexcept
{
	for(auto& c : *control_group)
		c.second.DrawTabOrder();

	HWND hwnd = GetHandle();
	HDC hdc = GetDC(hwnd);
	RECT rect;
	GetWindowRect(hwnd, &rect);
	Debug::DrawDebugTabOrder(0, rect, taborder);
	ReleaseDC(hwnd, hdc);

	InvalidateRect(hwnd, &rect, FALSE);
}

bool Control::BeforeKeyDown(HWND hwnd, WPARAM wparam) noexcept
{
	TextOut(GetDC(hwnd), 0, 0, TEXT("BELLLLLSDFHJJHLKSDJFLKSDJFLKSDJFDSLKFJL:KDJF"), 40);

	switch(wparam) {
		case VK_TAB: {
			DrawTabOrder();
			bool cycle_up = GetAsyncKeyState(VK_SHIFT);
			ControlGroup::CycleTab(cycle_up);
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
	SetWindowPos(hwnd, 0, position.x, position.y, window_size.cx, window_size.cy, SWP_NOZORDER);
	Show();
}
