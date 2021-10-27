#pragma once
#include "Control.hpp"

class ButtonControl : public Control {

public:
	ButtonControl(ButtonControl&&) noexcept;

	ButtonControl(const UserControlCreateInfo& ucci) noexcept;
	virtual ~ButtonControl() noexcept {}
};
