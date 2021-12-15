#ifndef SHOUJIN_SOURCE_GUI_SIZE
#define SHOUJIN_SOURCE_GUI_SIZE

#include "vector2d.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

struct Size : public Vector2d {
	Size() :
		Vector2d{} {}

	Size(int x, int y) :
		Vector2d{x, y} {}

	Size(const Vector2d& rhs) :
		Vector2d{rhs} {}

	Size(const POINT& rhs) :
		Vector2d{rhs} {}

	Size(const SIZE& rhs) :
		Vector2d{rhs} {}
};

}

#endif
