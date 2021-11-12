#include "../comctl32/edit_control.hpp"
#include "../comctl32/label_control.hpp"
#include "../layout/layout_stream.hpp"
#include "color_control.hpp"

using namespace shoujin::gui;
using namespace shoujin::gui::layout;

namespace shoujin::gui::comctl32 {

const Size ColorControl::DefaultClientSize{1024, 1024};

ColorControl::ColorControl(const LayoutParam& lp) :
	Window{BuildLayout(lp)}
{
	auto window = [](const LayoutParam& lp) -> Window* { auto p{lp}; p.tabstop=false; return new Window(p); };
	auto label = [](const LayoutParam& lp) -> Window* { return new LabelControl(lp); };
	auto edit = [](const LayoutParam& lp) -> Window* { return new EditControl(lp); };

	LayoutStream stream;

	stream
		<< Size{client_size() / 2} << layout::exstyle(WS_EX_CLIENTEDGE)
		<< topleft << create(this, window)
		<< layout::exstyle(0) << LabelControl::DefaultSize << unrelated << after
		<< create(this, label) << push << after << create(this, edit) << pop << below
		<< create(this, label) << push << after << create(this, edit) << pop << below
		<< create(this, label) << push << after << create(this, edit) << pop << below;
}

Window::CreateParam ColorControl::OnCreateParam()
{
	return CreateParam{.classname = TEXT("SHOUJIN_COLOR_CTRL")};
}

Window* ColorControl::Clone() const
{
	return new ColorControl(*this);
}

LayoutParam ColorControl::BuildLayout(const LayoutParam& lp)
{
	Size client_size;
	if(!lp.window_size && !lp.client_size)
		client_size = DefaultClientSize;

	LayoutParam layout = lp;
	layout.client_size = client_size;
	layout.exstyle = WS_EX_CLIENTEDGE;

	return layout;
}

}
