#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;
using namespace shoujin::gui::usercontrol;

void ColorControlDemoV1()
{
	ColorControl cc{LayoutParam{.layout_mode{LayoutMode::CenterParent}}};
	cc.ShowModal();
}
