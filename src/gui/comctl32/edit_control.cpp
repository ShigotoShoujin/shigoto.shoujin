#include "edit_control.hpp"

namespace shoujin::gui::comctl32 {

Size const EditControl::DefaultSize{160, 23};

EditControl::EditControl(LayoutParam const& lp) :
	Window{BuildLayout(lp)} {}

Window::CreateParam EditControl::OnCreateParam()
{
	return CreateParam{.classname = TEXT("EDIT"), .need_subclassing = true};
}

Window* EditControl::Clone() const
{
	return new EditControl(*this);
}

LayoutParam EditControl::BuildLayout(LayoutParam const& lp)
{
	LayoutParam layout = lp;

	if(!lp.window_size && !lp.client_size)
		layout.window_size = DefaultSize;

	layout.exstyle = WS_EX_CLIENTEDGE;

	return layout;
}

}
