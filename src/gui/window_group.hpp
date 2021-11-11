#pragma once
#define WIN32_LEAN_AND_MEAN
#include <map>
#include <Windows.h>

namespace shoujin::gui {

class Window;

class WindowGroup {
public:
	std::multimap<int, Window*> _taborder_map;
	int _taborder_max;

public:
	void AddWindow(Window* window, int& out_taborder);
	void CycleTab(bool cycle_up);

private:
	Window* FindNextWindowInTabOrder(Window* window, bool cycle_up) const;
	void SetFocusToFirstWindow();
};

}
