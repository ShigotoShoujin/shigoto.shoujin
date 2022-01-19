module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Window : Size;
import Shoujin.Geometry;

using shoujin::geometry::Vector2dI;

export namespace shoujin::gui2 {

struct Size : public Vector2dI {
	constexpr Size() = default;

	constexpr Size(int x, int y) :
		Vector2dI{x, y} {}

	constexpr Size(Vector2dI const& rhs) :
		Vector2dI{rhs} {}

	constexpr Size(POINT const& rhs) :
		Vector2dI{rhs.x, rhs.y} {}

	constexpr Size(SIZE const& rhs) :
		Vector2dI{rhs.cx, rhs.cy} {}
};

}
