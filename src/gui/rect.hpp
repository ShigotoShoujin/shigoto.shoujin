#pragma once
#include "point.hpp"
#include "size.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

struct Rect {
	int x1, y1, x2, y2;

	Rect() :
		x1{}, y1{}, x2{}, y2{} {}

	Rect(int x1, int y1, int x2, int y2) :
		x1{x1}, y1{y1}, x2{x2}, y2{y2} {}

	Rect(Point position, Size size) :
		x1{position.x}, y1{position.y}, x2{position.x + size.x}, y2{position.y + size.y} {}

	Rect(const RECT& rhs) :
		x1{rhs.left}, y1{rhs.top}, x2{rhs.right}, y2{rhs.bottom} {}

	[[nodiscard]] int width() const { return x2 - x1; }
	[[nodiscard]] int height() const { return y2 - y1; }

	operator bool() const { return x1 || y1 || x2 || y2; }
	operator RECT() const { return {x1, y1, x2, y2}; }

	friend bool operator==(const Rect& lhs, const Rect& rhs) { return lhs.x1 == rhs.x1 && lhs.y1 == rhs.y1 && lhs.x2 == rhs.x2 && lhs.y2 == rhs.y2; }
	friend bool operator!=(const Rect& lhs, const Rect& rhs) { return !(lhs == rhs); }
	friend Rect operator*(const Rect& lhs, int rhs) { return {lhs.x1, lhs.y1, lhs.x1 + lhs.width() * rhs, lhs.y1 + lhs.height() * rhs}; }
	friend Rect operator/(const Rect& lhs, int rhs) { return {lhs.x1, lhs.y1, lhs.x1 + lhs.width() / rhs, lhs.y1 + lhs.height() / rhs}; }
};

inline Size RectToSize(const Rect& rect) { return {rect.x2 - rect.x1, rect.y2 - rect.y1}; }
inline Size RectToSize(const RECT& rect) { return {rect.right - rect.left, rect.bottom - rect.top}; }

}
