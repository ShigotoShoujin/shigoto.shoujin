#include "label_control.hpp"

using namespace shoujin::gui;

namespace shoujin::gui::comctl32 {

Size const LabelControl::DefaultSize{80, 23};

LabelControl::LabelControl(LayoutParam const& lp) :
	Window{BuildLayout(lp)} {}

Window::CreateParam LabelControl::OnCreateParam()
{
	return CreateParam{.classname = TEXT("STATIC"), .need_subclassing{true}};
}

void LabelControl::SetLayout(Layout const& layout)
{
	auto lo = layout;
	lo.SetStyle(lo.style() | WS_BORDER, lo.exstyle());
	lo.SetTabStop(false);
	Window::SetLayout(lo);
}

Window* LabelControl::Clone() const
{
	return new LabelControl(*this);
}

LayoutParam LabelControl::BuildLayout(LayoutParam const& lp)
{
	LayoutParam layout = lp;

	if(!lp.window_size && !lp.client_size)
		layout.window_size = DefaultSize;

	layout.style = WS_BORDER;
	layout.tabstop = false;

	return layout;
}

}
