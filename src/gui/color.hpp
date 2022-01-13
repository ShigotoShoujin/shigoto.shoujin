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

	friend inline bool operator==(ColorByteRGB const& lhs, ColorByteRGB const& rhs)
	{
		return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue;
	}

	ColorByteRGB() = default;
	ColorByteRGB(int red, int green, int blue);
	ColorByteRGB(ColorFloatRGB const&);
	ColorByteRGB(ColorFloatHSL const&);
	ColorByteRGB(ColorFloatHSV const&);
};

struct ColorFloatRGB {
	float red{};
	float green{};
	float blue{};

	friend inline bool operator==(ColorFloatRGB const& lhs, ColorFloatRGB const& rhs)
	{
		return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue;
	}

	ColorFloatRGB() = default;
	ColorFloatRGB(float red, float green, float blue);
	ColorFloatRGB(ColorByteRGB const&);
	ColorFloatRGB(ColorFloatHSL const&);
	ColorFloatRGB(ColorFloatHSV const&);
};

struct ColorByteHSL {
	uint16_t hue{};
	uint8_t saturation{};
	uint8_t lightness{};

	friend inline bool operator==(ColorByteHSL const& lhs, ColorByteHSL const& rhs)
	{
		return lhs.hue == rhs.hue && lhs.saturation == rhs.saturation && lhs.lightness == rhs.lightness;
	}

	ColorByteHSL() = default;
	ColorByteHSL(int hue, int saturation, int lightness);
	ColorByteHSL(ColorFloatHSL const&);
	ColorByteHSL(ColorFloatRGB const&);
	ColorByteHSL(ColorFloatHSV const&);
};

struct ColorFloatHSL {
	float hue{};
	float saturation{};
	float lightness{};

	friend inline bool operator==(ColorFloatHSL const& lhs, ColorFloatHSL const& rhs)
	{
		return lhs.hue == rhs.hue && lhs.saturation == rhs.saturation && lhs.lightness == rhs.lightness;
	}

	ColorFloatHSL() = default;
	ColorFloatHSL(float hue, float saturation, float lightness);
	ColorFloatHSL(ColorByteHSL const&);
	ColorFloatHSL(ColorFloatRGB const&);
	ColorFloatHSL(ColorFloatHSV const&);
};

struct ColorByteHSV {
	uint16_t hue{};
	uint8_t saturation{};
	uint8_t value{};

	friend inline bool operator==(ColorByteHSV const& lhs, ColorByteHSV const& rhs)
	{
		return lhs.hue == rhs.hue && lhs.saturation == rhs.saturation && lhs.value == rhs.value;
	}

	ColorByteHSV() = default;
	ColorByteHSV(int hue, int saturation, int value);
	ColorByteHSV(ColorFloatHSV const&);
	ColorByteHSV(ColorFloatRGB const&);
	ColorByteHSV(ColorFloatHSL const&);
};

struct ColorFloatHSV {
	float hue{};
	float saturation{};
	float value{};

	friend inline bool operator==(ColorFloatHSV const& lhs, ColorFloatHSV const& rhs)
	{
		return lhs.hue == rhs.hue && lhs.saturation == rhs.saturation && lhs.value == rhs.value;
	}

	ColorFloatHSV() = default;
	ColorFloatHSV(float hue, float saturation, float value);
	ColorFloatHSV(ColorByteHSV const&);
	ColorFloatHSV(ColorFloatRGB const&);
	ColorFloatHSV(ColorFloatHSL const&);
};

class Color {
	ColorFloatRGB _color{};

public:
	Color() = default;
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
