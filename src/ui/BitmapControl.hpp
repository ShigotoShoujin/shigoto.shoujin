#pragma once
#include "Control.hpp"
#include "Bitmap.hpp"

class BitmapControl : public Control, public Bitmap {

public:
	BitmapControl(BitmapControl&&) noexcept;

	BitmapControl(const UserControlCreateInfo& ucci) noexcept;
	virtual ~BitmapControl() noexcept {}

	virtual bool OnPaint() noexcept override;
	virtual void SetParent(const Window& parent_window) noexcept override;
};
