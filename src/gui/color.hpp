#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h>

namespace shoujin::gui {
class Color {
	COLORREF _color;

	friend inline bool operator==(const Color& lhs, const Color& rhs);
	friend inline bool operator!=(const Color& lhs, const Color& rhs);

public:
	Color();
	Color(uint8_t r, uint8_t g, uint8_t b);
	Color(COLORREF color);

	operator COLORREF() const;

	static Color FromFloat(float r, float g, float b);

	//Basic colors taken from https://www.rapidtables.com/web/color/RGB_Color.html
	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Lime;
	static const Color Blue;
	static const Color Yellow;
	static const Color Cyan;
	static const Color Magenta;
	static const Color Silver;
	static const Color Gray;
	static const Color Maroon;
	static const Color Olive;
	static const Color Green;
	static const Color Purple;
	static const Color Teal;
	static const Color Navy;
};
}
