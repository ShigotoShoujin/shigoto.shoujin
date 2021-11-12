#include "label_control.hpp"

using namespace shoujin::gui;

namespace shoujin::gui::comctl32 {

const Size LabelControl::DefaultSize{80, 23};

LabelControl::LabelControl(const LayoutParam& lp) :
	Window{BuildLayout(lp)} {}

Window::CreateParam LabelControl::OnCreateParam()
{
	return CreateParam{.classname = TEXT("STATIC"), .need_subclassing{true}};
}

Window* LabelControl::Clone() const
{
	return new LabelControl(*this);
}

LayoutParam LabelControl::BuildLayout(const LayoutParam& lp)
{
	LayoutParam layout = lp;

	if(!lp.window_size && !lp.client_size)
		layout.window_size = DefaultSize;

	layout.style = WS_BORDER;
	layout.tabstop = false;

	return layout;
}

}
