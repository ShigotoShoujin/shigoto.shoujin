import Shoujin.Gui.Window;

using namespace shoujin::gui2;

void boot()
{
	Window w;
	
	auto z = w.handle();
	using HWND = decltype(z);
	
	HWND hwnd = w.handle();
}
