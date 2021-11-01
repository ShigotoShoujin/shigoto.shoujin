#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct Size {
	int x;
	int y;

	Size() :
		x{}, y{} {}

	Size(int x, int y) :
		x{x}, y{y} {}

	Size(const SIZE& size) :
		x{size.cx}, y{size.cy} {}

	inline operator bool() const { return x == 0 && y == 0; }
	inline operator SIZE() const { return {x, y}; }
};

inline bool operator==(const Size& lhs, const Size& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const Size& lhs, const Size& rhs)
{
	return !(lhs == rhs);
}

inline Size operator*(const Size& lhs, int rhs)
{
	return {lhs.x * rhs, lhs.y * rhs};
}

inline Size operator/(const Size& lhs, int rhs)
{
	return {lhs.x / rhs, lhs.y / rhs};
}
