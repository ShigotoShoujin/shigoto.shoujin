#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <shoujin/gui.hpp>

using namespace shoujin::gui;
using namespace shoujin::gui::usercontrol;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{
	ColorControl cc{LayoutParam{.layout_mode{LayoutMode::CenterParent}}};
	cc.ShowModal();

	return 0;
}
