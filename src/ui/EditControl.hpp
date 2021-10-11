#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Control.hpp"

class EditControl : public Control {
public:
	struct Args : public Control::Args {
	};

	EditControl(const Args& args) noexcept;
	virtual ~EditControl() {}

	virtual void SetFocus() noexcept override;
};
