#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <any>
#include <system_error>
#include "../event.hpp"
#include "../tstring.hpp"

namespace shoujin::assert {

extern bool _display_error_messagebox_;

extern Event<LPCTSTR, LPCTSTR, int, LPCTSTR, bool&> OnErrorEvent;
extern Event<tstring, bool&> OnErrorOutputEvent;
extern Event<bool&> OnExitProcessEvent;

void Abort(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression, std::any result);
void AbortCLib(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression, std::any result);
void AbortStdErrorCode(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression, std::any result);
void AbortWin32(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression, std::any result);

}

/// <summary>
/// Execute expression and capture its return value
/// <para>When the returned value is false, call abort_func</para>
/// <para>Meant to be used by other SHOUJIN_ASSERT macros</para>
/// <para>expression return type must convert implicitly to int</para>
/// </summary>
/// <returns>
/// Returns what expression evaluates to
/// </returns>
#define __SHOUJIN_ASSERT__(expression, abort_func) \
	[&]() { \
		auto result = expression; \
		if(!result) \
			abort_func(TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#expression), result); \
		return result; \
	}()

/// <summary>
/// Execute expression and capture its return value
/// <para>Call result_ok_func with the returned value</para>
/// <para>When result_ok_func returns false, call abort_func</para>
/// <para>Meant to be used by other SHOUJIN_ASSERT macros</para>
/// <para>expression return type must convert implicitly to int</para>
/// </summary>
/// <returns>
/// Returns what expression evaluates to
/// </returns>
#define __SHOUJIN_ASSERT_EXPLICIT__(expression, result_ok_func, abort_func) \
	[&]() { \
		auto result = expression; \
		if(!(result_ok_func(result))) \
			abort_func(TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__, TEXT(#expression), result); \
		return result; \
	}()

/// <summary>
/// Fails when expression evaluates to false
/// <para>expression return type must convert implicitly to int</para>
/// </summary>
/// <returns>Returns what expression evaluates to</returns>
#define SHOUJIN_ASSERT(expression) \
	__SHOUJIN_ASSERT__(expression, shoujin::assert::Abort)

/// <summary>
/// Calls result_ok_func with the result of expression as an argument
/// <para>Fails when result_ok_func returns false</para>
/// <para>expression return type must convert implicitly to int</para>
/// </summary>
/// <returns>Returns what expression evaluates to</returns>
#define SHOUJIN_ASSERT_EXPLICIT(expression, result_ok_func) \
	__SHOUJIN_ASSERT_EXPLICIT__(expression, result_ok_func, shoujin::assert::Abort)

/// <summary>
/// Fails when expression evaluates to false
/// <para>expression return type must convert implicitly to int</para>
/// </summary>
/// <returns>Returns what expression evaluates to</returns>
#define SHOUJIN_ASSERT_CLIB(expression) \
	__SHOUJIN_ASSERT_EXPLICIT__(expression, ([](auto ex) { return !ex; }), shoujin::assert::AbortCLib)

/// <summary>
/// Calls result_ok_func with the result of expression as an argument
/// <para>Fails when result_ok_func returns false</para>
/// <para>expression return type must convert implicitly to int</para>
/// </summary>
/// <returns>Returns what expression evaluates to</returns>
#define SHOUJIN_ASSERT_CLIB_EXPLICIT(expression, result_ok_func) \
	__SHOUJIN_ASSERT_EXPLICIT__(expression, result_ok_func, shoujin::assert::AbortCLib)

/// <summary>
/// Fails when expression evaluates to false
/// <para>expression return type must convert implicitly to std::error_code</para>
/// </summary>
/// <returns>Returns what expression evaluates to</returns>
#define SHOUJIN_ASSERT_STDERRORCODE(expression) \
	__SHOUJIN_ASSERT_EXPLICIT__(expression, ([](auto ex) { return !ex; }), shoujin::assert::AbortStdErrorCode)

/// <summary>
/// Calls result_ok_func with the result of expression as an argument
/// <para>Fails when result_ok_func returns false</para>
/// <para>expression return type must convert implicitly to std::error_code</para>
/// </summary>
/// <returns>Returns what expression evaluates to</returns>
#define SHOUJIN_ASSERT_STDERRORCODE_EXPLICIT(expression, result_ok_func) \
	__SHOUJIN_ASSERT_EXPLICIT__(expression, result_ok_func, shoujin::assert::AbortStdErrorCode)

/// <summary>
/// Fails when expression evaluates to false
/// <para>expression return type must convert implicitly to int</para>
/// </summary>
/// <returns>Returns what expression evaluates to</returns>
#define SHOUJIN_ASSERT_WIN32(expression) \
	__SHOUJIN_ASSERT__(expression, shoujin::assert::AbortWin32)

/// <summary>
/// Calls result_ok_func with the result of expression as an argument
/// <para>Fails when result_ok_func returns false</para>
/// <para>expression return type must convert implicitly to int</para>
/// </summary>
/// <returns>Returns what expression evaluates to</returns>
#define SHOUJIN_ASSERT_WIN32_EXPLICIT(expression, result_ok_func) \
	__SHOUJIN_ASSERT_EXPLICIT__(expression, result_ok_func, shoujin::assert::AbortWin32)
