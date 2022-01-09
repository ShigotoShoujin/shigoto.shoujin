#include "numeric_control.hpp"
#include <format>

namespace shoujin::gui::comctl32 {

NumericControl::NumericControl(LayoutParam const& layout_param) :
	EditControl{BuildLayout(layout_param)}
{
	_parent_onlayoutreset_event = OnLayoutResetEvent;
	OnLayoutResetEvent = {OnLayoutReset, this};
}

[[nodiscard]] int NumericControl::GetValue() const
{
	return ToInt(GetText());
}

int NumericControl::SetValue(int value) const
{
	return SetValue(value, GetText());
}

bool NumericControl::OnChange()
{
	if(!_onchange_depth) {
		EventDepth depth{_onchange_depth};
		auto text = GetText();
		SetValue(ToInt(text), text);
	}

	return Handled;
}

Window* NumericControl::Clone() const
{
	return new NumericControl(*this);
}

LayoutParam NumericControl::BuildLayout(LayoutParam layout)
{
	layout.style = ES_NUMBER;
	return layout;
}

void NumericControl::OnLayoutReset(Layout& layout, void* userdata)
{
	auto self = static_cast<NumericControl*>(userdata);
	self->_parent_onlayoutreset_event(layout);
	layout.SetStyle(layout.style() | ES_NUMBER, layout.exstyle());
}

int NumericControl::SetValue(int value, tstring const& previous_text) const
{
	if(_min_value && value < *_min_value)
		value = *_min_value;

	if(_max_value && value > *_max_value)
		value = *_max_value;

	auto new_text = std::format(TEXT("{:d}"), value);

	if(new_text != previous_text) {
		//Using WM_SETTEXT place the caret at the start of the EDIT, while replacing the selection place it at the end
		SendMessage(hwnd(), EM_SETSEL, 0, -1);
		SendMessage(hwnd(), EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(new_text.c_str()));
	}

	return value;
}

}
