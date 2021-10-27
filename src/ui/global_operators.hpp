#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

inline bool operator==(const SIZE& first, const SIZE& second)
{
	return first.cx == second.cx && first.cy == second.cy;
}
