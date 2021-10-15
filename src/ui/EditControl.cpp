#include "EditControl.hpp"

EditControl::EditControl(POINT position, SIZE size, LPCTSTR text) noexcept :
	Control{{.class_name = TEXT("EDIT"),
		.ex_style = WS_EX_STATICEDGE,
		.position = position,
		.text = text,
		.window_size = size}}
{}
