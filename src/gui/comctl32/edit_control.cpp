#include "edit_control.hpp"

namespace shoujin::gui::comctl32 {

const Size EditControl::DefaultClientSize{160, 16};

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
	Size client_size;
	if(!lp.window_size && !lp.client_size)
		client_size = DefaultClientSize;

	LayoutParam layout = lp;
	layout.client_size = client_size;
	layout.exstyle = WS_EX_CLIENTEDGE;

	return layout;
}

}
