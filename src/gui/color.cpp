#include "color.hpp"
#include <algorithm>
#include <stdint.h>

static uint8_t ToByte(float c)
{
	return static_cast<uint8_t>(0xff * std::clamp<float>(c, 0.f, 1.f));
}

static float ToFloat(uint8_t c)
{
	return std::clamp<float>(c, 0, 0xff) / 0xff;
}

namespace shoujin::gui {

Color::Color() :
	_color{}
{}

Color::Color(ColorByteRGB color) :
	_color{RGB(color.R, color.G, color.B)} {}

Color::Color(ColorFloatRGB color) :
	_color{RGB(ToByte(color.R), ToByte(color.G), ToByte(color.B))} {}

Color::Color(COLORREF color) :
	_color{color} {}

Color::operator ColorByteRGB() const
{
	return {
		GetRValue(_color),
		GetGValue(_color),
		GetBValue(_color)};
}

Color::operator ColorFloatRGB() const
{
	return {
		ToFloat(GetRValue(_color)),
		ToFloat(GetGValue(_color)),
		ToFloat(GetBValue(_color))};
}

Color::operator COLORREF() const
{
	return _color;
}

// clang-format off
Color const Color::Black   (ColorByteRGB {  0,   0,   0} );
Color const Color::White   (ColorByteRGB {255, 255, 255} );
Color const Color::Red     (ColorByteRGB {255,   0,   0} );
Color const Color::Lime    (ColorByteRGB {  0, 255,   0} );
Color const Color::Blue    (ColorByteRGB {  0,   0, 255} );
Color const Color::Yellow  (ColorByteRGB {255, 255,   0} );
Color const Color::Cyan    (ColorByteRGB {  0, 255, 255} );
Color const Color::Magenta (ColorByteRGB {255,   0, 255} );
Color const Color::Silver  (ColorByteRGB {192, 192, 192} );
Color const Color::Gray    (ColorByteRGB {128, 128, 128} );
Color const Color::Maroon  (ColorByteRGB {128,   0,   0} );
Color const Color::Olive   (ColorByteRGB {128, 128,   0} );
Color const Color::Green   (ColorByteRGB {  0, 128,   0} );
Color const Color::Purple  (ColorByteRGB {128,   0, 128} );
Color const Color::Teal    (ColorByteRGB {  0, 128, 128} );
Color const Color::Navy    (ColorByteRGB {  0,   0, 128} );
// clang-format on

}
