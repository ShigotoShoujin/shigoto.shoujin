#ifndef SHOUJIN_SOURCE_GUI_RECT
#define SHOUJIN_SOURCE_GUI_RECT

#include "point.hpp"
#include "size.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

struct Rect {
	int x1, y1, x2, y2;

	constexpr Rect() :
		x1{}, y1{}, x2{}, y2{} {}

	constexpr Rect(int x1, int y1, int x2, int y2) :
		x1{x1}, y1{y1}, x2{x2}, y2{y2} {}

	constexpr Rect(Point position, Size size) :
		x1{position.x}, y1{position.y}, x2{position.x + size.x}, y2{position.y + size.y} {}

	constexpr Rect(RECT const& rhs) :
		x1{rhs.left}, y1{rhs.top}, x2{rhs.right}, y2{rhs.bottom} {}

	[[nodiscard]] constexpr int width() const { return x2 - x1; }
	[[nodiscard]] constexpr int height() const { return y2 - y1; }

	constexpr operator bool() const { return x1 || y1 || x2 || y2; }
	constexpr operator RECT() const { return {x1, y1, x2, y2}; }

	constexpr friend bool operator==(Rect const& lhs, Rect const& rhs) { return lhs.x1 == rhs.x1 && lhs.y1 == rhs.y1 && lhs.x2 == rhs.x2 && lhs.y2 == rhs.y2; }
	constexpr friend bool operator!=(Rect const& lhs, Rect const& rhs) { return !(lhs == rhs); }
	constexpr friend Rect operator*(Rect const& lhs, int rhs) { return {lhs.x1, lhs.y1, lhs.x1 + lhs.width() * rhs, lhs.y1 + lhs.height() * rhs}; }
	constexpr friend Rect operator/(Rect const& lhs, int rhs) { return {lhs.x1, lhs.y1, lhs.x1 + lhs.width() / rhs, lhs.y1 + lhs.height() / rhs}; }
};

constexpr inline Size RectToSize(Rect const& rect) { return {rect.x2 - rect.x1, rect.y2 - rect.y1}; }
constexpr inline Size RectToSize(RECT const& rect) { return {rect.right - rect.left, rect.bottom - rect.top}; }

}

#endif
