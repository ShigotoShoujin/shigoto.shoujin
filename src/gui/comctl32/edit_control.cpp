#include "edit_control.hpp"

namespace shoujin::gui::comctl32 {

static void OnLayoutReset(Layout& layout, void* userdata);

Size const EditControl::DefaultSize{160, 23};

EditControl::EditControl(LayoutParam const& layout_param) :
	Comctl32{TEXT("EDIT"), BuildLayout(layout_param)}
{
	OnLayoutResetEvent = {OnLayoutReset, this};
}

bool EditControl::autoselect() const
{
	return _autoselect;
}

bool EditControl::readonly() const
{
	return _readonly;
}

void EditControl::autoselect(bool value)
{
	_autoselect = value;
}

void EditControl::readonly(bool value)
{
	_readonly = value;

	if(hwnd())
		SendMessage(hwnd(), EM_SETREADONLY, _readonly, 0);
}

void EditControl::OnInitialize()
{
	readonly(_readonly);
}

bool EditControl::OnCommand(int notification_code)
{
	switch(notification_code) {
		case EN_CHANGE:
			return RaiseOnChange();
		case EN_SETFOCUS:
			if(_autoselect)
				PostMessage(hwnd(), EM_SETSEL, 0, -1);
			return true;
	}

	return false;
}

bool EditControl::OnChange()
{
	return NotHandled;
}

bool EditControl::OnKeyPress(KeyEvent const& e)
{
	if(e.virtual_keycode == VK_TAB)
		return Handled;

	return NotHandled;
}

Window::MessageResult EditControl::RaiseOnChange()
{
	auto result = OnChange();
	return result | (OnChangeEvent ? OnChangeEvent(this) : NotHandled);
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
