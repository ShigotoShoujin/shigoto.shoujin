module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Types : Point;
import : Vector2d;

export namespace shoujin::gui2 {

struct Point : public Vector2d {
	constexpr Point() :
		Vector2d{} {}

	constexpr Point(int x, int y) :
		Vector2d{x, y} {}

	constexpr Point(Vector2d const& rhs) :
		Vector2d{rhs} {}

	constexpr Point(POINT const& rhs) :
		Vector2d{rhs} {}

	constexpr Point(SIZE const& rhs) :
		Vector2d{rhs} {}

	constexpr Point& ClampPoint(Vector2d const& size)
	{
		if(x < 0)
			x = 0;
		else if(x >= size.x)
			x = size.x - 1;

		if(y < 0)
			y = 0;
		else if(y >= size.y)
			y = size.y - 1;

		return *this;
	}
};

}
