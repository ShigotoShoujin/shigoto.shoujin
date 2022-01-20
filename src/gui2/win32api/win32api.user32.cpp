module;
#include "../../assert/assert.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Win32Api : User32;

export namespace shoujin::gui2::win32api {

BOOL adjustWindowRectEx(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle)
{
	return SHOUJIN_ASSERT_WIN32(::AdjustWindowRectEx(lpRect, dwStyle, bMenu, dwExStyle));
}

HWND createWindowEx(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	return SHOUJIN_ASSERT_WIN32(CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam));
}

LRESULT dispatchMessage(const MSG* lpMsg)
{
	return DispatchMessage(lpMsg);
}

BOOL getClassInfoEx(HINSTANCE hInstance, LPCTSTR lpszClass, LPWNDCLASSEX lpwcx)
{
	return GetClassInfoEx(hInstance, lpszClass, lpwcx);
}

BOOL getMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	auto okFunc = [](auto result) { auto const getMessageError = -1; return result != getMessageError; };
	return SHOUJIN_ASSERT_WIN32_EXPLICIT(GetMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax), okFunc);
}

[[nodiscard]] int getSystemMetrics(int nIndex)
{
	return SHOUJIN_ASSERT(::GetSystemMetrics(nIndex));
}

BOOL peekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	return PeekMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

ATOM registerClassEx(CONST WNDCLASSEX* wcx)
{
	return SHOUJIN_ASSERT_WIN32(RegisterClassEx(wcx));
}

BOOL translateMessage(CONST MSG* lpMsg)
{
	return ::TranslateMessage(lpMsg);
}

BOOL showWindow(HWND hWnd, int nCmdShow)
{
	return ::ShowWindow(hWnd, nCmdShow);
}

BOOL updateWindow(HWND hWnd)
{
	return SHOUJIN_ASSERT(::UpdateWindow(hWnd));
}

}
