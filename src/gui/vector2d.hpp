#pragma once
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

	Vector2d(const POINT& rhs) :
		x{rhs.x}, y{rhs.y} {}

	Vector2d(const SIZE& rhs) :
		x{rhs.cx}, y{rhs.cy} {}

	operator bool() const { return x != 0 || y != 0; }
	operator POINT() const { return {x, y}; }
	operator SIZE() const { return {x, y}; }

	friend bool operator==(const Vector2d& lhs, const Vector2d& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
	friend bool operator!=(const Vector2d& lhs, const Vector2d& rhs) { return !(lhs == rhs); }
	friend Vector2d operator*(const Vector2d& lhs, int rhs) { return {lhs.x * rhs, lhs.y * rhs}; }
	friend Vector2d operator/(const Vector2d& lhs, int rhs) { return {lhs.x / rhs, lhs.y / rhs}; }
};

}
