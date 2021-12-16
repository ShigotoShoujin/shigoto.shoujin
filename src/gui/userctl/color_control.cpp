#include "../../assert/assert.hpp"
#include "../bitmap_window.hpp"
#include "../comctl32/edit_control.hpp"
#include "../comctl32/label_control.hpp"
#include "../layout/layout_stream.hpp"
#include "color_control.hpp"

using namespace shoujin::gui;
using namespace shoujin::gui::layout;

namespace shoujin::gui::comctl32 {

Size const ColorControl::DefaultClientSize{768, 768};

ColorControl::ColorControl(LayoutParam const& lp) :
	Window{BuildLayout(lp)}
{
	auto bitmap_window = [](LayoutParam const& lp) -> Window* { auto p{lp}; p.tabstop = false; return new BitmapWindow(p); };
	auto window = [](LayoutParam const& lp) -> Window* { auto p{lp}; p.tabstop=false; return new Window(p); };
	auto label = [](LayoutParam const& lp) -> Window* { return new LabelControl(lp); };
	auto edit = [](LayoutParam const& lp) -> Window* { return new EditControl(lp); };

	LayoutStream stream;

	stream
		<< Size{client_size() / 2} << layout::exstyle(WS_EX_CLIENTEDGE)
		<< topleft << create(this, bitmap_window)
		<< layout::exstyle(0) << LabelControl::DefaultSize << unrelated << after
		<< TEXT("Red") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below
		<< TEXT("Green") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below
		<< TEXT("Blue") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below;

	AddChild(new EditControl(LayoutParam{.anchor{AnchorRight | AnchorBottom}}));
	AddChild(new EditControl(LayoutParam{.anchor{AnchorRight | AnchorTop}}));
	AddChild(new EditControl(LayoutParam{.anchor{AnchorLeft | AnchorBottom}}));
}

Window::CreateParam ColorControl::OnCreateParam()
{
	return CreateParam{.classname = TEXT("ShoujinColorControl")};
}

Window* ColorControl::Clone() const
{
	return new ColorControl(*this);
}

LayoutParam ColorControl::BuildLayout(LayoutParam const& lp)
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
