#pragma once
#include "vector2d.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

struct Point : public Vector2d {
	Point() :
		Vector2d{} {}

	Point(int x, int y) :
		Vector2d{x, y} {}

	Point(const Vector2d& rhs) :
		Vector2d{rhs} {}

	Point(const POINT& rhs) :
		Vector2d{rhs} {}

	Point(const SIZE& rhs) :
		Vector2d{rhs} {}
};

}
