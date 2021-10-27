#include "BitmapControl.hpp"
#include "ColorPickerControl.hpp"

ColorPickerControl::ColorPickerControl(ColorPickerControl&& other) noexcept :
	Control{std::move(other)} {}

ColorPickerControl::ColorPickerControl(const UserControlCreateInfo& ucci) noexcept :
	Control{ucci, 0, 0, WS_EX_CLIENTEDGE}
{
	AddChild(new BitmapControl(UserControlCreateInfo{.layout = Window::Layout::FillParent}));
}
