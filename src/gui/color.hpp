#ifndef SHOUJIN_SOURCE_GUI_COLOR
#define SHOUJIN_SOURCE_GUI_COLOR

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h>

namespace shoujin::gui {

class Color {
	COLORREF _color;

	friend inline bool operator==(Color const& lhs, Color const& rhs);
	friend inline bool operator!=(Color const& lhs, Color const& rhs);

public:
	Color();
	Color(uint8_t r, uint8_t g, uint8_t b);
	Color(COLORREF color);

	[[nodiscard]] inline uint8_t red() const { return GetRValue(_color); }
	[[nodiscard]] inline uint8_t green() const { return GetGValue(_color); }
	[[nodiscard]] inline uint8_t blue() const { return GetBValue(_color); }

	operator COLORREF() const;

	static Color FromFloat(float r, float g, float b);

	//Basic colors taken from https://www.rapidtables.com/web/color/RGB_Color.html
	static Color const Black;
	static Color const White;
	static Color const Red;
	static Color const Lime;
	static Color const Blue;
	static Color const Yellow;
	static Color const Cyan;
	static Color const Magenta;
	static Color const Silver;
	static Color const Gray;
	static Color const Maroon;
	static Color const Olive;
	static Color const Green;
	static Color const Purple;
	static Color const Teal;
	static Color const Navy;
};

}

#endif
