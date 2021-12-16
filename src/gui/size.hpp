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

	Size(Vector2d const& rhs) :
		Vector2d{rhs} {}

	Size(POINT const& rhs) :
		Vector2d{rhs} {}

	Size(SIZE const& rhs) :
		Vector2d{rhs} {}
};

}

#endif
