#pragma once
#include "Control.hpp"

class ColorControl : public Control {

public:
	ColorControl(ColorControl&&) noexcept;

	ColorControl(const UserControlCreateInfo& ucci) noexcept;
	virtual ~ColorControl() noexcept {}
};
