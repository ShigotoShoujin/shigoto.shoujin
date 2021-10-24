#include "BitmapControl.hpp"
#include "ColorControl.hpp"

ColorControl::ColorControl(ColorControl&& other) noexcept :
	Control{std::move(other)} {}

ColorControl::ColorControl(const UserControlCreateInfo& ucci) noexcept :
	Control{ucci, 0, 0, WS_EX_CLIENTEDGE}
{
	AddChild(new BitmapControl(UserControlCreateInfo{.layout = Window::Layout::FillParent}));
}
