#include "edit_control.hpp"

namespace shoujin::gui::comctl32 {

static void OnLayoutReset(Layout& layout, void* userdata);

Size const EditControl::DefaultSize{160, 23};

EditControl::EditControl(LayoutParam const& layout_param) :
	Comctl32{TEXT("EDIT"), BuildLayout(layout_param)}
{
	OnLayoutResetEvent = {OnLayoutReset, this};
}

bool EditControl::selectall_on_focus() const
{
	return _selectall_on_focus;
}

void EditControl::selectall_on_focus(bool value)
{
	_selectall_on_focus = value;
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

static void OnLayoutReset(Layout& layout, void* userdata)
{
	layout.SetStyle(layout.style(), layout.exstyle() | WS_EX_CLIENTEDGE);
}

}
