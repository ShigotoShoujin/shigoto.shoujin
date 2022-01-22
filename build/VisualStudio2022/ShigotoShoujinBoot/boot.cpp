import Shoujin.Gui.Window;

#include <array>
#include <Windows.h>

using namespace shoujin::gui2;

void twoWindows()
{
	Window w;

	w.show();

	Window w2 = w;
	w2.show();

	auto list = std::array<Window*, 2>{&w, &w2};

	bool more;
	do {
		more = false;
		for(auto&& it : list)
			more |= it->processMessageQueue();
	} while(more);
}


void boot()
{
	twoWindows();
}
