#include "edit_control.hpp"

namespace shoujin::gui::comctl32 {

const Size EditControl::DefaultSize{160, 23};

EditControl::EditControl(const LayoutParam& lp) :
	Window{BuildLayout(lp)} {}

Window::CreateParam EditControl::OnCreateParam()
{
	return CreateParam{.classname = TEXT("EDIT"), .need_subclassing = true};
}

Window* EditControl::Clone() const
{
	return new EditControl(*this);
}

LayoutParam EditControl::BuildLayout(const LayoutParam& lp)
{
	LayoutParam layout = lp;

	if(!lp.window_size && !lp.client_size)
		layout.window_size = DefaultSize;

	layout.exstyle = WS_EX_CLIENTEDGE;

	return layout;
}

}
