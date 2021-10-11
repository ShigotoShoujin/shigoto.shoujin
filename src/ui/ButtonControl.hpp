#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Control.hpp"

class ButtonControl : public Control {
public:
	struct Args : public Control::Args {
	};

	ButtonControl(const Args& args) noexcept;
	virtual ~ButtonControl() {}
};
