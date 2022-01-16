module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Types : Size;
import : Vector2d;

export namespace shoujin::gui2 {

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
