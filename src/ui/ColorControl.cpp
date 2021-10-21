#include "ColorControl.hpp"

ColorControl::ColorControl(ColorControl&& other) noexcept :
	Control{std::move(other)} {}

ColorControl::ColorControl(const UserControlCreateInfo& ucci) noexcept :
	Control{ucci, TEXT("ShigotoShoujinColorControl"), WS_BORDER, WS_EX_CLIENTEDGE}
	//Control{ucci, TEXT("EDIT"), 0, WS_EX_CLIENTEDGE}
	//Control{ucci, TEXT("EDIT"), 0, WS_EX_CLIENTEDGE}
{}
