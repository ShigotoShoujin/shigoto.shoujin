#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <system_error>
#include "../event.hpp"
#include "../tstring.hpp"

namespace shoujin::assert {
extern bool display_error_messagebox;

extern Event<LPCTSTR, LPCTSTR, int, LPCTSTR, bool&> OnErrorEvent;
extern Event<tstring, bool&> OnErrorOutputEvent;
extern Event<bool&> OnExitProcessEvent;

void Abort(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression);
void AbortCLib(int errcode, LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression);
void AbortStdErrorCode(std::error_code std_error_code, LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression);
void AbortWin32(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression);
}

#ifdef NDEBUG
#include <cassert>
#define SHOUJIN_ASSERT(fn) assert(fn)
#define SHOUJIN_ASSERT_CLIB(expected, fn) assert(fn)
#define SHOUJIN_ASSERT_STDERRORCODE(std_error_code) assert(fn)
#define SHOUJIN_ASSERT_WIN32(fn) assert(fn)
#define SHOUJIN_ASSERT_WIN32_EX(fn, isok_func) assert(isok_func((fn)))
#else
#define SHOUJIN_ASSERT(fn) \
	if(!(fn)) \
	shoujin::assert::Abort(TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#fn))

#define SHOUJIN_ASSERT_CLIB(expected, fn) \
	{ \
		auto errcode = (fn); \
		if(expected != errcode) \
			shoujin::assert::AbortCLib(errcode, TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#fn)); \
	}

#define SHOUJIN_ASSERT_STDERRORCODE(std_error_code) \
	if(std_error_code) \
	shoujin::assert::AbortStdErrorCode(std_error_code, TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#std_error_code))

#define SHOUJIN_ASSERT_WIN32(fn) \
	[&]() { \
		auto ret = (fn); \
		if(!ret) \
			shoujin::assert::AbortWin32(TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#fn)); \
		return ret; \
	}()

#define SHOUJIN_ASSERT_WIN32_EX(fn, isok_func) \
	[&]() { \
		auto ret = (fn); \
		if(!(isok_func(ret))) \
			shoujin::assert::AbortWin32(TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#fn)); \
		return ret; \
	}()
#endif
