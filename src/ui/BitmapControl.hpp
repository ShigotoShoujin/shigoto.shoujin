#pragma once
#include "Control.hpp"
#include "Bitmap.hpp"

class BitmapControl : public Control {

public:
	BitmapControl(BitmapControl&&) noexcept;

	BitmapControl(const UserControlCreateInfo& ucci) noexcept;
	virtual ~BitmapControl() noexcept {}
};
