#ifndef SHOUJIN_SOURCE_GUI_COLOR
#define SHOUJIN_SOURCE_GUI_COLOR

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>

namespace shoujin::gui {

struct ColorByteRGB {
	uint8_t R{}; //Red
	uint8_t G{}; //Green
	uint8_t B{}; //Blue
};

struct ColorFloatRGB {
	float R{}; //Red
	float G{}; //Green
	float B{}; //Blue
};

struct ColorByteHSL {
	uint16_t H{}; //Hue
	uint8_t S{}; //Saturation
	uint8_t L{}; //Lightness
};

struct ColorFloatHSL {
	float H{}; //Hue
	float S{}; //Saturation
	float L{}; //Lightness
};

class Color {
	COLORREF _color;

public:
	Color();
	Color(COLORREF color);
	Color(ColorByteRGB color);
	Color(ColorFloatRGB color);
	Color(ColorByteHSL color);
	Color(ColorFloatHSL color);

	operator COLORREF() const;
	operator ColorByteRGB() const;
	operator ColorFloatRGB() const;
	operator ColorByteHSL() const;
	operator ColorFloatHSL() const;

	friend inline bool operator==(Color const& lhs, Color const& rhs)
	{
		return lhs._color == rhs._color;
	}

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
