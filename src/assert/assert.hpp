#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::assert {
extern void (*ExitProcessFunc)(_In_ UINT uExitCode);
__declspec(noreturn) void Abort(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression);
__declspec(noreturn) void AbortCLib(int errcode, LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression);
__declspec(noreturn) void AbortWin32(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression);
}

#ifdef NDEBUG
#include <cassert>
#define ASSERT(fn) assert(fn)
#define ASSERT_CLIB(fn) assert(fn)
#define ASSERT_WIN32(fn) assert(fn)
#else
#define ASSERT(fn) \
	if(!(fn)) \
	shoujin::assert::Abort(TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#fn))

#define ASSERT_CLIB(expected, fn) \
	{ \
		auto result = (fn); \
		if(expected != result) \
			shoujin::assert::AbortCLib(result, TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#fn)); \
	}

#define ASSERT_WIN32(fn) \
	if(!(fn)) \
	shoujin::assert::AbortWin32(TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#fn))
#endif
