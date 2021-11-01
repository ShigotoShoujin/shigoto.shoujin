#include "color.hpp"
#include <algorithm>
#include <stdint.h>

namespace shoujin::gui {

Color::Color() :
	_color{}
{}

Color::Color(uint8_t r, uint8_t g, uint8_t b) :
	_color{RGB(r, g, b)} {}

Color::Color(COLORREF color) :
	_color{color} {}

Color::operator COLORREF() const
{
	return _color;
}

bool operator==(const Color& lhs, const Color& rhs)
{
	return lhs._color == rhs._color;
}

bool operator!=(const Color& lhs, const Color& rhs)
{
	return !(lhs._color == rhs._color);
}

Color Color::FromFloat(float r, float g, float b)
{
	auto ftoi8 = [](float v) -> uint8_t { return static_cast<uint8_t>(0xff * std::clamp<float>(v, 0.f, 1.f)); };
	return Color(ftoi8(r), ftoi8(g), ftoi8(b));
}

const Color Color::Black(0, 0, 0);
const Color Color::White(255, 255, 255);
const Color Color::Red(255, 0, 0);
const Color Color::Lime(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Cyan(0, 255, 255);
const Color Color::Magenta(255, 0, 255);
const Color Color::Silver(192, 192, 192);
const Color Color::Gray(128, 128, 128);
const Color Color::Maroon(128, 0, 0);
const Color Color::Olive(128, 128, 0);
const Color Color::Green(0, 128, 0);
const Color Color::Purple(128, 0, 128);
const Color Color::Teal(0, 128, 128);
const Color Color::Navy(0, 0, 128);

}
