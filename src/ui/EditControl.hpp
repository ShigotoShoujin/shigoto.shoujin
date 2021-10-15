#pragma once
#include "Control.hpp"

class EditControl : public Control {
	DISABLE_COPY(EditControl)

public:
	EditControl(POINT position, SIZE size, LPCTSTR text) noexcept;
	virtual ~EditControl() noexcept {}
};
