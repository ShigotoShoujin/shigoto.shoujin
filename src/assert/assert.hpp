#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <system_error>

namespace shoujin::assert {
extern void (*ExitProcessFunc)(_In_ UINT uExitCode);
__declspec(noreturn) void Abort(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression);
__declspec(noreturn) void AbortCLib(int errcode, LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression);
__declspec(noreturn) void AbortStdErrorCode(std::error_code std_error_code, LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression);
__declspec(noreturn) void AbortWin32(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression);
}

#ifdef NDEBUG
#include <cassert>
#define ASSERT(fn) assert(fn)
#define ASSERT_CLIB(expected, fn) assert(fn)
#define ASSERT_STDERRORCODE(std_error_code) assert(fn)
#define ASSERT_WIN32(fn) assert(fn)
#else
#define ASSERT(fn) \
	if(!(fn)) \
	shoujin::assert::Abort(TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#fn))

#define ASSERT_CLIB(expected, fn) \
	{ \
		auto errcode = (fn); \
		if(expected != errcode) \
			shoujin::assert::AbortCLib(errcode, TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#fn)); \
	}

#define ASSERT_STDERRORCODE(std_error_code) \
	if(std_error_code) \
		shoujin::assert::AbortStdErrorCode(std_error_code, TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#std_error_code));

#define ASSERT_WIN32(fn) \
	if(!(fn)) \
	shoujin::assert::AbortWin32(TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#fn))
#endif
