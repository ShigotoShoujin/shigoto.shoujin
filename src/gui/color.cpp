#include "color.hpp"
#include <algorithm>
#include <cmath>

using namespace shoujin::gui;

static uint8_t ToByte(float channel);
static float ToFloat(uint8_t channel);
static ColorFloatHSL ToHSL(ColorFloatRGB rgb);
static ColorFloatHSV ToHSV(ColorFloatRGB rgb);
static ColorFloatRGB ToRGB(ColorFloatHSL hsl);
static ColorFloatRGB ToRGB(ColorFloatHSV hsv);

namespace shoujin::gui {

ColorByteRGB::ColorByteRGB(int red, int green, int blue) :
	red{static_cast<uint8_t>(red)},
	green{static_cast<uint8_t>(green)},
	blue{static_cast<uint8_t>(blue)}
{}

ColorByteRGB::ColorByteRGB(ColorFloatRGB const& cfrgb) :
	red{static_cast<uint8_t>(cfrgb.red * 255)},
	green{static_cast<uint8_t>(cfrgb.green * 255)},
	blue{static_cast<uint8_t>(cfrgb.blue * 255)}
{}

ColorFloatRGB::ColorFloatRGB(float red, float green, float blue) :
	red{red},
	green{green},
	blue{blue}
{}

ColorFloatRGB::ColorFloatRGB(ColorByteRGB const& cbhsl) :
	red{cbhsl.red / 255.f},
	green{cbhsl.green / 255.f},
	blue{cbhsl.blue / 255.f}
{}

ColorByteHSL::ColorByteHSL(int hue, int saturation, int lightness) :
	hue{static_cast<uint16_t>(hue)},
	saturation{static_cast<uint8_t>(saturation)},
	lightness{static_cast<uint8_t>(lightness)}
{}

ColorByteHSL::ColorByteHSL(ColorFloatHSL const& cfhsl) :
	hue{static_cast<uint16_t>(cfhsl.hue)},
	saturation{static_cast<uint8_t>(cfhsl.saturation * 100)},
	lightness{static_cast<uint8_t>(cfhsl.lightness * 100)}
{}

ColorFloatHSL::ColorFloatHSL(float hue, float saturation, float lightness) :
	hue{hue},
	saturation{saturation},
	lightness{lightness}
{}

ColorFloatHSL::ColorFloatHSL(ColorByteHSL const& cbhsl) :
	hue{cbhsl.hue * 1.f},
	saturation{cbhsl.saturation / 100.f},
	lightness{cbhsl.lightness / 100.f}
{}

ColorByteHSV::ColorByteHSV(int hue, int saturation, int value) :
	hue{static_cast<uint16_t>(hue)},
	saturation{static_cast<uint8_t>(saturation)},
	value{static_cast<uint8_t>(value)}
{}

ColorByteHSV::ColorByteHSV(ColorFloatHSV const& cfhsv) :
	hue{static_cast<uint16_t>(cfhsv.hue)},
	saturation{static_cast<uint8_t>(cfhsv.saturation * 100)},
	value{static_cast<uint8_t>(cfhsv.value * 100)}
{}

ColorFloatHSV::ColorFloatHSV(float hue, float saturation, float value) :
	hue{hue},
	saturation{saturation},
	value{value}
{}

ColorFloatHSV::ColorFloatHSV(ColorByteHSV const& cbhsv) :
	hue{cbhsv.hue * 1.f},
	saturation{cbhsv.saturation / 100.f},
	value{cbhsv.value / 100.f}
{}

Color::Color() :
	_color{}
{}

Color::Color(COLORREF color) :
	_color{color} {}

Color::Color(ColorByteRGB color) :
	_color{RGB(color.red, color.green, color.blue)} {}

Color::Color(ColorFloatRGB color) :
	_color{RGB(ToByte(color.red), ToByte(color.green), ToByte(color.blue))} {}

Color::Color(ColorByteHSL color) :
	_color{Color{ToRGB(color)}} {}

Color::Color(ColorFloatHSL color) :
	_color{Color{ToRGB(color)}} {}

Color::Color(ColorByteHSV color) :
	_color{Color{ToRGB(color)}} {}

Color::Color(ColorFloatHSV color) :
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
	return ToHSL(*this);
}

Color::operator ColorFloatHSL() const
{
	return ToHSL(*this);
}

Color::operator ColorByteHSV() const
{
	return ToHSV(*this);
}

Color::operator ColorFloatHSV() const
{
	return ToHSV(*this);
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
	auto max = max(max(rgb.red, rgb.green), rgb.blue);
	auto min = min(min(rgb.red, rgb.green), rgb.blue);
	auto delta = max - min;

	//Hue
	float H;
	if(delta == 0.f)
		H = 0.f;
	else if(max == rgb.red)
		H = 60.f * fmodf((rgb.green - rgb.blue) / delta, 6.f);
	else if(max == rgb.green)
		H = 60.f * ((rgb.blue - rgb.red) / delta + 2);
	else if(max == rgb.blue)
		H = 60.f * ((rgb.red - rgb.green) / delta + 4);

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

static ColorFloatHSV ToHSV(ColorFloatRGB rgb)
{
	return {};
}

static ColorFloatRGB ToRGB(ColorFloatHSL hsl)
{
	float C = (1 - abs(2 * hsl.lightness - 1)) * hsl.saturation;
	float X = C * (1 - abs(fmodf(hsl.hue / 60.f, 2.f) - 1));
	float m = hsl.lightness - C / 2;

	ColorFloatRGB rgb;

	switch(static_cast<int>(hsl.hue) / 60) {
		case 6:
		case 0: rgb = {C, X, 0}; break;
		case 1: rgb = {X, C, 0}; break;
		case 2: rgb = {0, C, X}; break;
		case 3: rgb = {0, X, C}; break;
		case 4: rgb = {X, 0, C}; break;
		case 5: rgb = {C, 0, X};
	}

	rgb.red += m;
	rgb.green += m;
	rgb.blue += m;

	return rgb;
}

static ColorFloatRGB ToRGB(ColorFloatHSV hsv)
{
	return {};
}
