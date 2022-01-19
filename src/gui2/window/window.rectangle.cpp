module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Window : Rectangle;
import : Point;
import : Size;
import Shoujin.Geometry;

using shoujin::geometry::RectangleI;

export namespace shoujin::gui2 {

struct Rectangle : public RectangleI {
	constexpr Rectangle(int x1, int y1, int x2, int y2) :
		RectangleI{x1, y1, x2, y2} {}

	constexpr Rectangle(Point const& position, Size const& size) :
		RectangleI{position.x, position.y, size.x, size.y} {}

	constexpr Rectangle(RECT const& rhs) :
		RectangleI{rhs.left, rhs.top, rhs.right, rhs.bottom} {}

	constexpr operator RECT() const { return {x1, y1, x2, y2}; }
};

constexpr inline Size RectangleToSize(RECT const& rect) { return {rect.right - rect.left, rect.bottom - rect.top}; }

}
