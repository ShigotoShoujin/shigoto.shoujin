#include "color.hpp"
#include <algorithm>

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

Color::Color(COLORREF color) :
	_color{color} {}

Color::Color(ColorByteRGB color) :
	_color{RGB(color.R, color.G, color.B)} {}

Color::Color(ColorFloatRGB color) :
	_color{RGB(ToByte(color.R), ToByte(color.G), ToByte(color.B))} {}

Color::operator COLORREF() const
{
	return _color;
}

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

Color::operator ColorHSL() const
{
	ColorFloatRGB rgb = *this;
	auto max = max(max(rgb.R, rgb.G), rgb.B);
	auto min = min(min(rgb.R, rgb.G), rgb.B);
	auto delta = max - min;

	//Hue
	float H;
	if(delta == 0.f)
		H = 0.f;
	else if(max == rgb.R)
		H = 60.f * (static_cast<int>((rgb.G - rgb.B) / delta) % 6);
	else if(max == rgb.G)
		H = 60.f * ((rgb.B - rgb.R) / delta + 2);
	else if(max == rgb.B)
		H = 60.f * ((rgb.R - rgb.G) / delta + 4);

	//Lightness
	float L = (max + min) / 2;

	//Saturation
	float S = delta == 0.f ? 0.f : delta / (1 - abs(2 * L - 1));

	return {H, S, L};
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
