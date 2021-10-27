#pragma once
#include "Control.hpp"

class EditControl : public Control {

public:
	EditControl(EditControl&&) noexcept;

	EditControl(const UserControlCreateInfo& ucci) noexcept;
	virtual ~EditControl() noexcept {}
};
