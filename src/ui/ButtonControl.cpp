#include "ButtonControl.hpp"

ButtonControl::ButtonControl(ButtonControl&& other) noexcept :
	Control{std::move(other)} {}

ButtonControl::ButtonControl(const UserControlCreateInfo& ucci) noexcept :
	Control{ucci, TEXT("BUTTON"), 0, WS_EX_STATICEDGE}
{}
