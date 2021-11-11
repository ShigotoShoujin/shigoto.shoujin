//#include "../assert/assert.hpp"
#include "../assert/assert.hpp"
#include "window.hpp"
#include "window_group.hpp"

namespace shoujin::gui {

void WindowGroup::AddWindow(Window* window, int& out_taborder)
{
	SHOUJIN_ASSERT(window);

	if(window->tabstop()) {
		_taborder_map.emplace(++_taborder_max, window);
		out_taborder = _taborder_max;
	}
	else
		out_taborder = 0;
}

void WindowGroup::CycleTab(bool cycle_up)
{
	HWND focus_hwnd = GetFocus();
	if(!focus_hwnd) {
		SetFocusToFirstWindow();
		return;
	}

	auto window = Window::FindWindowByHandle(focus_hwnd);
	if(!window) {
		SetFocusToFirstWindow();
		return;
	}

	window = FindNextWindowInTabOrder(window, cycle_up);
	if(window)
		window->SetFocus();
}

Window* WindowGroup::FindNextWindowInTabOrder(Window* window, bool cycle_up) const
{
	if(_taborder_map.size() == 1)
		return window;

	auto current = _taborder_map.find(window->taborder());
	if(current == _taborder_map.end())
		return nullptr;

	if(cycle_up) {
		if(current == _taborder_map.begin())
			return _taborder_map.rbegin()->second;

		return std::prev(current)->second;
	}

	auto next = std::next(current);
	if(next == _taborder_map.end())
		return _taborder_map.begin()->second;

	return next->second;
}

void WindowGroup::SetFocusToFirstWindow()
{
	const auto it = _taborder_map.cbegin();
	if(it != _taborder_map.cend())
		it->second->SetFocus();
}

}
