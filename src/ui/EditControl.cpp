#include "EditControl.hpp"

EditControl::EditControl(EditControl&& other) noexcept :
	Control{std::move(other)} {}

EditControl::EditControl(const UserControlCreateInfo& ucci) noexcept :
	Control{ucci, TEXT("EDIT"), 0, WS_EX_STATICEDGE}
{}
