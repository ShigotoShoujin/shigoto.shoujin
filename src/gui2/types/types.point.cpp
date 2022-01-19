module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Types : Point;
import Shoujin.Geometry;

using shoujin::geometry::Vector2dI;

export namespace shoujin::gui2 {

struct Point : public Vector2dI {
	constexpr Point() :
		Vector2dI{} {}

	constexpr Point(int x, int y) :
		Vector2dI{x, y} {}

	constexpr Point(Vector2dI const& rhs) :
		Vector2dI{rhs} {}

	constexpr Point(POINT const& rhs) :
		Vector2dI{rhs.x, rhs.y} {}

	constexpr Point(SIZE const& rhs) :
		Vector2dI{rhs.cx, rhs.cy} {}
};

}
