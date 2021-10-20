#pragma once
#include "Control.hpp"

class ButtonControl : public Control {

public:
	ENABLE_MOVE_CONSTRUCTOR(ButtonControl)

	ButtonControl(const UserControlCreateInfo& ucci) noexcept;
	virtual ~ButtonControl() noexcept {}
};
