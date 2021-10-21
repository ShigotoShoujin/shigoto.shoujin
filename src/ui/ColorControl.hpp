#pragma once
#include "Control.hpp"

class ColorControl : public Control {

public:
	ENABLE_MOVE_CONSTRUCTOR(ColorControl)

	ColorControl(const UserControlCreateInfo& ucci) noexcept;
	virtual ~ColorControl() noexcept {}
};
