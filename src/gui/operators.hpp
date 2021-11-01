#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

inline bool operator==(const POINT& lhs, int rhs)
{
	return lhs.x == rhs && lhs.y == rhs;
}

inline bool operator==(const POINT& lhs, const POINT& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const POINT& lhs, const POINT& rhs)
{
	return !(lhs == rhs);
}

inline bool operator==(const SIZE& lhs, int rhs)
{
	return lhs.cx == rhs && lhs.cy == rhs;
}

inline bool operator==(const SIZE& lhs, const SIZE& rhs)
{
	return lhs.cx == rhs.cx && lhs.cy == rhs.cy;
}

inline bool operator!=(const SIZE& lhs, const SIZE& rhs)
{
	return !(lhs == rhs);
}

inline SIZE operator/(const SIZE& lhs, int rhs)
{
	return {lhs.cx / rhs, lhs.cy / rhs};
}
