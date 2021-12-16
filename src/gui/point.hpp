#ifndef SHOUJIN_SOURCE_GUI_POINT
#define SHOUJIN_SOURCE_GUI_POINT

#include "vector2d.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

struct Point : public Vector2d {
	Point() :
		Vector2d{} {}

	Point(int x, int y) :
		Vector2d{x, y} {}

	Point(Vector2d const& rhs) :
		Vector2d{rhs} {}

	Point(POINT const& rhs) :
		Vector2d{rhs} {}

	Point(SIZE const& rhs) :
		Vector2d{rhs} {}
};

}

#endif
