#include "color.hpp"
#include <algorithm>
#include <cmath>

using namespace shoujin::gui;

static uint8_t ToByte(float channel);
static float ToFloat(uint8_t channel);
static ColorFloatHSL ToHSL(ColorFloatRGB rgb);
static ColorFloatRGB ToRGB(ColorFloatHSL hsl);

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

Color::Color(ColorByteHSL color) :
	_color{Color{ToRGB(ColorFloatHSL{static_cast<float>(color.H), static_cast<float>(color.S) / 100, static_cast<float>(color.L) / 100})}} {}

Color::Color(ColorFloatHSL color) :
	_color{Color{ToRGB(color)}} {}

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

Color::operator ColorByteHSL() const
{
	auto cfhsl = ToHSL(*this);
	return {
		static_cast<uint16_t>(cfhsl.H),
		static_cast<uint8_t>(cfhsl.S * 100),
		static_cast<uint8_t>(cfhsl.L * 100)};
}

Color::operator ColorFloatHSL() const
{
	return ToHSL(*this);
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

static uint8_t ToByte(float channel)
{
	auto b = 0xff * std::clamp<float>(channel, 0.f, 1.f);
	return static_cast<uint8_t>(std::roundf(b));
}

static float ToFloat(uint8_t channel)
{
	return std::clamp<float>(channel, 0, 0xff) / 0xff;
}

static ColorFloatHSL ToHSL(ColorFloatRGB rgb)
{
	auto max = max(max(rgb.R, rgb.G), rgb.B);
	auto min = min(min(rgb.R, rgb.G), rgb.B);
	auto delta = max - min;

	//Hue
	float H;
	if(delta == 0.f)
		H = 0.f;
	else if(max == rgb.R)
		H = 60.f * fmodf((rgb.G - rgb.B) / delta, 6.f);
	else if(max == rgb.G)
		H = 60.f * ((rgb.B - rgb.R) / delta + 2);
	else if(max == rgb.B)
		H = 60.f * ((rgb.R - rgb.G) / delta + 4);

	//While converting RGB(255, 254, 255), H will be -60 but should be 300.
	//Fixed by adding 360 when H is negative.
	if(H < 0)
		H += 360;

	//Lightness
	float L = (max + min) / 2;

	//Saturation
	float S = delta == 0.f ? 0.f : delta / (1 - abs(2 * L - 1));

	return {H, S, L};
}

static ColorFloatRGB ToRGB(ColorFloatHSL hsl)
{
	float C = (1 - abs(2 * hsl.L - 1)) * hsl.S;
	float X = C * (1 - abs(fmodf(hsl.H / 60.f, 2.f) - 1));
	float m = hsl.L - C / 2;

	ColorFloatRGB rgb;

	switch(static_cast<int>(hsl.H) / 60) {
		case 0: rgb = {C, X, 0}; break;
		case 1: rgb = {X, C, 0}; break;
		case 2: rgb = {0, C, X}; break;
		case 3: rgb = {0, X, C}; break;
		case 4: rgb = {X, 0, C}; break;
		case 5: rgb = {C, 0, X};
	}

	rgb.R += m;
	rgb.G += m;
	rgb.B += m;

	return rgb;
}
