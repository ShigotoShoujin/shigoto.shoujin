#include "edit_control.hpp"

namespace shoujin::gui::comctl32 {

Size const EditControl::DefaultSize{160, 23};

EditControl::EditControl(LayoutParam const& lp) :
	Window{BuildLayout(lp)} {}

bool EditControl::selectall_on_focus() const
{
	return _selectall_on_focus;
}

void EditControl::selectall_on_focus(bool value)
{
	_selectall_on_focus = value;
}

Window::CreateParam EditControl::OnCreateParam()
{
	return CreateParam{.classname = TEXT("EDIT"), .need_subclassing = true};
}

bool EditControl::OnCommand(int notification_code)
{
	switch(notification_code) {
		case EN_SETFOCUS:
			if(_selectall_on_focus)
				PostMessage(hwnd(), EM_SETSEL, 0, -1);
			return true;
	}

	return false;
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
