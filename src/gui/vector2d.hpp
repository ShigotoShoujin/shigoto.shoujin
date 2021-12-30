#ifndef SHOUJIN_SOURCE_GUI_VECTOR2D
#define SHOUJIN_SOURCE_GUI_VECTOR2D

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

struct Vector2d {
	int x;
	int y;

	Vector2d() :
		x{}, y{} {}

	Vector2d(int x, int y) :
		x{x}, y{y} {}

	Vector2d(POINT const& rhs) :
		x{rhs.x}, y{rhs.y} {}

	Vector2d(SIZE const& rhs) :
		x{rhs.cx}, y{rhs.cy} {}

	operator bool() const { return x || y; }
	operator POINT() const { return {x, y}; }
	operator SIZE() const { return {x, y}; }

	friend bool operator==(Vector2d const& lhs, Vector2d const& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
	friend bool operator!=(Vector2d const& lhs, Vector2d const& rhs) { return !(lhs == rhs); }

	friend Vector2d operator+(Vector2d const& lhs, Vector2d const& rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y}; }
	friend Vector2d operator-(Vector2d const& lhs, Vector2d const& rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }

	friend Vector2d operator*(Vector2d const& lhs, int rhs) { return {lhs.x * rhs, lhs.y * rhs}; }
	friend Vector2d operator/(Vector2d const& lhs, int rhs) { return {lhs.x / rhs, lhs.y / rhs}; }
};

}

#endif
