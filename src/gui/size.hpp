#ifndef SHOUJIN_SOURCE_GUI_SIZE
#define SHOUJIN_SOURCE_GUI_SIZE

#include "vector2d.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

struct Size : public Vector2d {
	constexpr Size() :
		Vector2d{} {}

	constexpr Size(int x, int y) :
		Vector2d{x, y} {}

	constexpr Size(Vector2d const& rhs) :
		Vector2d{rhs} {}

	constexpr Size(POINT const& rhs) :
		Vector2d{rhs} {}

	constexpr Size(SIZE const& rhs) :
		Vector2d{rhs} {}
};

}

#endif
