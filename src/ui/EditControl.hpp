#pragma once
#include "Control.hpp"

class EditControl : public Control {

public:
	ENABLE_MOVE_CONSTRUCTOR(EditControl)

	EditControl(const UserControlCreateInfo& ucci) noexcept;
	EditControl(POINT position, SIZE size, LPCTSTR text) noexcept;
	virtual ~EditControl() noexcept {}
};
