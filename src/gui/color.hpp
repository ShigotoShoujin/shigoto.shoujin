#ifndef SHOUJIN_SOURCE_GUI_COLOR
#define SHOUJIN_SOURCE_GUI_COLOR

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>

namespace shoujin::gui {

struct ColorByteRGB;
struct ColorByteHSL;
struct ColorByteHSV;
struct ColorFloatRGB;
struct ColorFloatHSL;
struct ColorFloatHSV;

struct ColorByteRGB {
	uint8_t red{};
	uint8_t green{};
	uint8_t blue{};

	ColorByteRGB() = default;
	ColorByteRGB(int red, int green, int blue);
	ColorByteRGB(ColorFloatRGB const& cfrgb);
};

struct ColorFloatRGB {
	float red{};
	float green{};
	float blue{};

	ColorFloatRGB() = default;
	ColorFloatRGB(float red, float green, float blue);
	ColorFloatRGB(ColorByteRGB const& cbhsl);
};

struct ColorByteHSL {
	uint16_t hue{};
	uint8_t saturation{};
	uint8_t lightness{};

	ColorByteHSL() = default;
	ColorByteHSL(int hue, int saturation, int lightness);
	ColorByteHSL(ColorFloatHSL const& cfhsl);
};

struct ColorFloatHSL {
	float hue{};
	float saturation{};
	float lightness{};

	ColorFloatHSL() = default;
	ColorFloatHSL(float hue, float saturation, float lightness);
	ColorFloatHSL(ColorByteHSL const& cbhsl);
};

struct ColorByteHSV {
	uint16_t hue{};
	uint8_t saturation{};
	uint8_t value{};

	ColorByteHSV() = default;
	ColorByteHSV(int hue, int saturation, int value);
	ColorByteHSV(ColorFloatHSV const& cfhsv);
};

struct ColorFloatHSV {
	float hue{};
	float saturation{};
	float value{};

	ColorFloatHSV() = default;
	ColorFloatHSV(float hue, float saturation, float value);
	ColorFloatHSV(ColorByteHSV const& cbhsv);
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
	Color(ColorByteHSV color);
	Color(ColorFloatHSV color);

	operator COLORREF() const;
	operator ColorByteRGB() const;
	operator ColorFloatRGB() const;
	operator ColorByteHSL() const;
	operator ColorFloatHSL() const;
	operator ColorByteHSV() const;
	operator ColorFloatHSV() const;

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
