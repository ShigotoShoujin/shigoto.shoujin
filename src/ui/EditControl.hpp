#pragma once
#include "Control.hpp"

class EditControl : public Control {

public:
	ENABLE_MOVE(EditControl);

	EditControl(POINT position, SIZE size, LPCTSTR text) noexcept;
	virtual ~EditControl() noexcept {}
};
