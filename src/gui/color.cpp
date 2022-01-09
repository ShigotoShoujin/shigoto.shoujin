#include "color.hpp"
#include <algorithm>
#include <cmath>

using namespace shoujin::gui;

static ColorFloatHSL ToHSL(ColorFloatRGB const& rgb);
static ColorFloatHSV ToHSV(ColorFloatRGB const& rgb);
static ColorFloatRGB ToRGB(ColorFloatHSL const& hsl);
static ColorFloatRGB ToRGB(ColorFloatHSV const& hsv);

namespace shoujin::gui {

ColorByteRGB::ColorByteRGB(int red, int green, int blue) :
	red{static_cast<uint8_t>(red)},
	green{static_cast<uint8_t>(green)},
	blue{static_cast<uint8_t>(blue)}
{}

ColorByteRGB::ColorByteRGB(ColorFloatRGB const& cfrgb) :
	red{static_cast<uint8_t>(std::roundf(cfrgb.red * 255))},
	green{static_cast<uint8_t>(std::roundf(cfrgb.green * 255))},
	blue{static_cast<uint8_t>(std::roundf(cfrgb.blue * 255))}
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
	hue{static_cast<uint16_t>(std::roundf(cfhsl.hue))},
	saturation{static_cast<uint8_t>(std::roundf(cfhsl.saturation * 100))},
	lightness{static_cast<uint8_t>(std::roundf(cfhsl.lightness * 100))}
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
	hue{static_cast<uint16_t>(std::roundf(cfhsv.hue))},
	saturation{static_cast<uint8_t>(std::roundf(cfhsv.saturation * 100))},
	value{static_cast<uint8_t>(std::roundf(cfhsv.value * 100))}
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

Color::Color(COLORREF color) :
	_color{ColorByteRGB{static_cast<int>(GetRValue(color)), static_cast<int>(GetGValue(color)), static_cast<int>(GetBValue(color))}} {}

Color::Color(ColorByteRGB color) :
	_color{color} {}

Color::Color(ColorFloatRGB color) :
	_color{color} {}

Color::Color(ColorByteHSL color) :
	_color{ToRGB(color)} {}

Color::Color(ColorFloatHSL color) :
	_color{ToRGB(color)} {}

Color::Color(ColorByteHSV color) :
	_color{ToRGB(color)} {}

Color::Color(ColorFloatHSV color) :
	_color{ToRGB(color)} {}

Color::operator COLORREF() const
{
	ColorByteRGB rgb{_color};
	return RGB(rgb.red, rgb.green, rgb.blue);
}

Color::operator ColorByteRGB() const
{
	return _color;
}

Color::operator ColorFloatRGB() const
{
	return _color;
}

Color::operator ColorByteHSL() const
{
	return ToHSL(_color);
}

Color::operator ColorFloatHSL() const
{
	return ToHSL(_color);
}

Color::operator ColorByteHSV() const
{
	return ToHSV(_color);
}

Color::operator ColorFloatHSV() const
{
	return ToHSV(_color);
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

static void CHMMRGB(ColorFloatRGB const& rgb, float& H, float& C, float& min, float& max)
{
	min = min(min(rgb.red, rgb.green), rgb.blue);
	max = max(max(rgb.red, rgb.green), rgb.blue);
	C = max - min;

	if(C == 0.f)
		H = 0.f;
	else if(max == rgb.red)
		H = 60.f * ((rgb.green - rgb.blue) / C);
	else if(max == rgb.green)
		H = 60.f * ((rgb.blue - rgb.red) / C + 2);
	else if(max == rgb.blue)
		H = 60.f * ((rgb.red - rgb.green) / C + 4);

	//While converting RGB(255, 254, 255), H will be -60 but should be 300.
	//Fixed by adding 360 when H is negative.
	if(H < 0)
		H += 360;
}

static ColorFloatHSL ToHSL(ColorFloatRGB const& rgb)
{
	float H, C, min, max;

	CHMMRGB(rgb, H, C, min, max);
	float L = (max + min) / 2;
	float S = C == 0.f ? 0.f : C / (1 - abs(2 * L - 1));

	return {H, S, L};
}

static ColorFloatHSV ToHSV(ColorFloatRGB const& rgb)
{
	float H, C, min, max;

	CHMMRGB(rgb, H, C, min, max);
	float L = max;
	float S = C == 0.f ? 0.f : C / L;

	return {H, S, L};
}

static ColorFloatRGB HCXmRGB(float H, float C, float X, float m)
{
	ColorFloatRGB rgb;

	switch(static_cast<int>(H)) {
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

static ColorFloatRGB ToRGB(ColorFloatHSL const& hsl)
{
	auto H = hsl.hue / 60.f;
	float C = (1 - abs(2 * hsl.lightness - 1)) * hsl.saturation;
	float X = C * (1 - abs(std::fmodf(H, 2) - 1));
	float m = hsl.lightness - C / 2;
	return HCXmRGB(H, C, X, m);
}

static ColorFloatRGB ToRGB(ColorFloatHSV const& hsv)
{
	auto H = hsv.hue / 60.f;
	float C = hsv.value * hsv.saturation;
	float X = C * (1 - abs(std::fmodf(H, 2) - 1));
	float m = hsv.value - C;

	return HCXmRGB(H, C, X, m);
}
