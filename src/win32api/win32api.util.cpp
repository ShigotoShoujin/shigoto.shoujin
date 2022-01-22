module;
#include <type_traits>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Win32Api : Util;
import : User32;

export namespace shoujin::win32api::util {

template<typename T> static T getWindowPtr(HWND hwnd, int index)
{
	if constexpr(std::is_arithmetic_v<T>)
		return static_cast<T>(getWindowLongPtr(hwnd, index));
	else
		return reinterpret_cast<T>(getWindowLongPtr(hwnd, index));
}

std::vector<TCHAR> getWindowTextVector(HWND hWnd, size_t extraCharsToAlloc = {})
{
	size_t textLength = getWindowTextLength(hWnd);
	textLength += extraCharsToAlloc;

	if(textLength == 0)
		return {};

	std::vector<TCHAR> buffer(textLength + 1);
	getWindowText(hWnd, buffer.data(), static_cast<int>(textLength) + 1);

	return buffer;
}

auto setWindowPtr(HWND hWnd, int index, auto newValue) -> decltype(newValue)
{
	LONG_PTR previousValue = setWindowLongPtr(hWnd, index, reinterpret_cast<LONG_PTR>(newValue));
	return reinterpret_cast<decltype(newValue)>(previousValue);
}

}
