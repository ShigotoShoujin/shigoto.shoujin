#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include "../tstring.hpp"

#ifdef UNICODE
#define TCERR std::wcerr
#else
#define TCERR std::cerr
#endif

namespace shoujin::assert {

/*static*/ void (*ExitProcessFunc)(_In_ UINT uExitCode) = nullptr;

tstring GetFileFromPath(tstring file)
{
	return file.substr(file.find_last_of(TEXT("/\\")) + 1);
}

tstring FormatError(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression)
{
	tstringstream ss;

	ss
		<< TEXT("File: ") << GetFileFromPath(file) << std::endl
		<< TEXT("Function: ") << function << std::endl
		<< TEXT("Line: ") << line << std::endl
		<< TEXT("Expression: ") << expression << std::endl;

	return ss.str();
}

static void ExitProcess(UINT exit_code)
{
	ExitProcessFunc ? ExitProcessFunc(exit_code) : ::ExitProcess(exit_code);
}

__declspec(noreturn) void Abort(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression)
{
	TCERR << FormatError(file, function, line, expression);
	ExitProcess(1);
}

__declspec(noreturn) void AbortCLib(int errcode, LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression)
{
	const int MSG_BUFFER_SIZE = 0xff;
	TCHAR msg_buffer[MSG_BUFFER_SIZE];
	tstring custom_error;

	if(errcode && !_tcserror_s(msg_buffer, MSG_BUFFER_SIZE, errcode)) {
		tstringstream ss;
		ss
			<< TEXT("CLib error code: ") << errcode << std::endl
			<< TEXT("CLib error message: ") << msg_buffer << std::endl;
		custom_error = ss.str();
	}

	TCERR
		<< FormatError(file, function, line, expression)
		<< custom_error;

	ExitProcess(errcode ? errcode : 1);
}

__declspec(noreturn) void AbortWin32(LPCTSTR file, LPCTSTR function, int line, LPCTSTR expression)
{
	DWORD last_error = GetLastError();
	LPTSTR msg_buffer;
	tstring custom_error;

	DWORD size_in_tchar;

	if(last_error && (size_in_tchar = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, last_error, 0, reinterpret_cast<LPTSTR>(&msg_buffer), 0, 0))) {
		tstringstream ss;
		ss
			<< TEXT("Win32 error code: ") << last_error << std::endl
			<< TEXT("Win32 error message: ") << msg_buffer << std::endl;
		custom_error = ss.str();
		LocalFree(msg_buffer);
	}

	TCERR
		<< FormatError(file, function, line, expression)
		<< custom_error;

	ExitProcess(last_error ? last_error : 1);
}

}
