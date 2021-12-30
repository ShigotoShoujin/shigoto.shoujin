#include "edit_control.hpp"

namespace shoujin::gui::comctl32 {

Size const EditControl::DefaultSize{160, 23};

EditControl::EditControl(LayoutParam const& lp) :
	Window{BuildLayout(lp)} {}

Window::CreateParam EditControl::OnCreateParam()
{
	return CreateParam{.classname = TEXT("EDIT"), .need_subclassing = true};
}

void EditControl::SetLayout(Layout const& layout)
{
	auto lo = layout;
	lo.SetStyle(lo.style(), lo.exstyle() | WS_EX_CLIENTEDGE);
	Window::SetLayout(lo);
}

Window* EditControl::Clone() const
{
	return new EditControl(*this);
}

LayoutParam EditControl::BuildLayout(LayoutParam layout)
{
	if(!layout.window_size && !layout.client_size)
		layout.window_size = DefaultSize;

	layout.exstyle = WS_EX_CLIENTEDGE;

	return layout;
}

}
