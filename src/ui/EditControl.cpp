#include "EditControl.hpp"

EditControl::EditControl(EditControl&& other) noexcept :
	Control{std::move(other)} {}

EditControl& EditControl::operator=(EditControl&& other) noexcept
{
	if(this == &other)
		return *this;

	Control::operator=(std::move(other));
	return *this;
}

EditControl::EditControl(POINT position, SIZE size, LPCTSTR text) noexcept :
	Control{{.class_name = TEXT("EDIT"),
		.ex_style = WS_EX_STATICEDGE,
		.position = position,
		.text = text,
		.window_size = size}}
{}
