#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <filesystem>
#include <string>
#include "../event.hpp"
#include "../tstring.hpp"
#include "../file/file.hpp"

#ifdef UNICODE
#define TCERR std::wcerr
#else
#define TCERR std::cerr
#endif

namespace shoujin::assert {

Event<LPCTSTR, LPCTSTR, int, LPCTSTR, bool&> OnErrorEvent;
Event<tstring, bool&> OnErrorOutputEvent;
Event<bool&> OnExitProcessEvent;

static bool OnError(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression)
{
	bool cancel = false;

	if(OnErrorEvent)
		OnErrorEvent(file, function, line, expression, cancel);

	return !cancel;
}

static bool OnErrorOutput(tstring error_message)
{
	bool cancel = false;

	if(OnErrorOutputEvent)
		OnErrorOutputEvent(error_message, cancel);

	if(!cancel)
		TCERR << error_message;

	return !cancel;
}

static void OnExitProcess(UINT exit_code)
{
	bool cancel = false;

	if(OnExitProcessEvent)
		OnExitProcessEvent(cancel);

	if(!cancel)
		::ExitProcess(exit_code);
}

static tstring FormatError(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression)
{
	tstringstream ss;
	std::filesystem::path path(file);

	ss
		<< TEXT("File: ") << path.filename() << std::endl
		<< TEXT("Function: ") << function << std::endl
		<< TEXT("Line: ") << line << std::endl
		<< TEXT("Expression: ") << expression << std::endl;

	return ss.str();
}

void Abort(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression)
{
	if(!OnError(file, function, line, expression))
		return;

	tstringstream ss;
	ss << FormatError(file, function, line, expression);

	if(OnErrorOutput(ss.str()))
		OnExitProcess(1);
}

void AbortCLib(int errcode, LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression)
{
	if(!OnError(file, function, line, expression))
		return;

	tstringstream ss;
	const int MSG_BUFFER_SIZE = 0xff;
	TCHAR msg_buffer[MSG_BUFFER_SIZE];

	ss << FormatError(file, function, line, expression);

	if(errcode && !_tcserror_s(msg_buffer, MSG_BUFFER_SIZE, errcode)) {
		ss
			<< TEXT("CLib error code: ") << errcode << std::endl
			<< TEXT("CLib error message: ") << msg_buffer << std::endl;
	}

	if(OnErrorOutput(ss.str()))
		OnExitProcess(errcode ? errcode : 1);
}

void AbortStdErrorCode(std::error_code std_error_code, LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression)
{
	if(!OnError(file, function, line, expression))
		return;

	tstringstream ss;

	ss
		<< FormatError(file, function, line, expression)
		<< TEXT("std::error_code::value: ") << std_error_code.value() << std::endl
		<< TEXT("std::error_code::message: ") << std_error_code.message().c_str() << std::endl;

	if(OnErrorOutput(ss.str()))
		OnExitProcess(std_error_code.value());
}

void AbortWin32(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression)
{
	if(!OnError(file, function, line, expression))
		return;

	tstringstream ss;
	DWORD last_error = GetLastError();
	LPTSTR msg_buffer;
	DWORD size_in_tchar;

	ss << FormatError(file, function, line, expression);

	if(last_error && (size_in_tchar = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, last_error, 0, reinterpret_cast<LPTSTR>(&msg_buffer), 0, 0))) {
		ss
			<< TEXT("Win32 error code: ") << last_error << std::endl
			<< TEXT("Win32 error message: ") << msg_buffer << std::endl;
		LocalFree(msg_buffer);
	}

	if(OnErrorOutput(ss.str()))
		OnExitProcess(last_error ? last_error : 1);
}

}
