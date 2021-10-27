#pragma once
#include "Control.hpp"

class ColorPickerControl : public Control {

public:
	ColorPickerControl(ColorPickerControl&&) noexcept;

	ColorPickerControl(const UserControlCreateInfo& ucci) noexcept;
	virtual ~ColorPickerControl() noexcept {}
};
