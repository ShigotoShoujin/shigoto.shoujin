#include "ColorControl.hpp"

ColorControl::ColorControl(ColorControl&& other) noexcept :
	Control{std::move(other)} {}

ColorControl::ColorControl(const UserControlCreateInfo& ucci) noexcept :
	Control{ucci, 0, WS_BORDER, WS_EX_CLIENTEDGE}
{}
