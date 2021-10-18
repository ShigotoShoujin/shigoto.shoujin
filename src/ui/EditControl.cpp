#include "EditControl.hpp"

EditControl::EditControl(EditControl&& other) noexcept :
	Control{std::move(other)} {}

EditControl::EditControl(const UserControlCreateInfo& ucci) noexcept :
	Control{ucci, TEXT("EDIT"), 0, WS_EX_STATICEDGE}
{}

EditControl::EditControl(POINT position, SIZE size, LPCTSTR text) noexcept :
	Control{{.class_name = TEXT("EDIT"),
		.ex_style = WS_EX_STATICEDGE,
		.position = position,
		.text = text,
		.window_size = size}}
{}
