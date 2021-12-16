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

bool operator==(Color const& lhs, Color const& rhs)
{
	return lhs._color == rhs._color;
}

bool operator!=(Color const& lhs, Color const& rhs)
{
	return !(lhs._color == rhs._color);
}

Color Color::FromFloat(float r, float g, float b)
{
	auto ftoi8 = [](float v) -> uint8_t { return static_cast<uint8_t>(0xff * std::clamp<float>(v, 0.f, 1.f)); };
	return Color(ftoi8(r), ftoi8(g), ftoi8(b));
}

Color const Color::Black(0, 0, 0);
Color const Color::White(255, 255, 255);
Color const Color::Red(255, 0, 0);
Color const Color::Lime(0, 255, 0);
Color const Color::Blue(0, 0, 255);
Color const Color::Yellow(255, 255, 0);
Color const Color::Cyan(0, 255, 255);
Color const Color::Magenta(255, 0, 255);
Color const Color::Silver(192, 192, 192);
Color const Color::Gray(128, 128, 128);
Color const Color::Maroon(128, 0, 0);
Color const Color::Olive(128, 128, 0);
Color const Color::Green(0, 128, 0);
Color const Color::Purple(128, 0, 128);
Color const Color::Teal(0, 128, 128);
Color const Color::Navy(0, 0, 128);

}
