#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

struct Point {
	int x;
	int y;

	Point() :
		x{}, y{} {}

	Point(int x, int y) :
		x{x}, y{y} {}

	Point(const POINT& size) :
		x{size.x}, y{size.y} {}

	operator bool() const { return x != 0 || y != 0; }
	operator POINT() const { return {x, y}; }

	friend bool operator==(const Point& lhs, const Point& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
	friend bool operator!=(const Point& lhs, const Point& rhs) { return !(lhs == rhs); }
	friend Point operator*(const Point& lhs, int rhs) { return {lhs.x * rhs, lhs.y * rhs}; }
	friend Point operator/(const Point& lhs, int rhs) { return {lhs.x / rhs, lhs.y / rhs}; }
};

}
