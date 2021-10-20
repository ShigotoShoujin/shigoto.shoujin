#pragma once
#include "Control.hpp"

class EditControl : public Control {

public:
	ENABLE_MOVE_CONSTRUCTOR(EditControl)

	EditControl(const UserControlCreateInfo& ucci) noexcept;
	virtual ~EditControl() noexcept {}
};
