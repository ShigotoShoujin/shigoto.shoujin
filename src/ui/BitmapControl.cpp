#include "BitmapControl.hpp"

BitmapControl::BitmapControl(BitmapControl&& other) noexcept :
	Control{std::move(other)} {}

BitmapControl::BitmapControl(const UserControlCreateInfo& ucci) noexcept :
	Control{ucci, 0, WS_BORDER, WS_EX_CLIENTEDGE}
{}
