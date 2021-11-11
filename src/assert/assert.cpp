#include "../event.hpp"
#include "../file/file.hpp"
#include "../tstring.hpp"
#include "assert.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <tchar.h>

#ifdef UNICODE
#define TCERR std::wcerr
#else
#define TCERR std::cerr
#endif

namespace shoujin::assert {

thread_local bool _activate_assert_messagebox_;

Event<bool, const ErrorInfo&> OnErrorEvent;
Event<bool, tstring> OnErrorOutputEvent;
Event<bool> OnExitProcessEvent;

static bool OnError(const ErrorInfo& ei)
{
	return OnErrorEvent(ei);
}

static bool OnErrorOutput(tstring error_message)
{
	bool cancel = false;

	if(OnErrorOutputEvent)
		cancel = OnErrorOutputEvent(error_message);

	if(!cancel) {
		TCERR << error_message;
		if(_activate_assert_messagebox_) {
			LPCTSTR text = error_message.c_str();
			MessageBox(HWND_DESKTOP, text, TEXT("Shoujin Assert"), MB_OK | MB_ICONWARNING);
		}
	}

	return cancel;
}

static void OnExitProcess(UINT exit_code)
{
	bool cancel = false;

	if(OnExitProcessEvent)
		cancel = OnExitProcessEvent();

	if(!cancel)
		::ExitProcess(exit_code);
}

static tstring FormatError(const ErrorInfo& ei)
{
	tstringstream ss;
	std::filesystem::path path(ei.file);

	ss
		<< TEXT("File: ") << path.filename() << std::endl
		<< TEXT("Function: ") << ei.function << std::endl
		<< TEXT("Line: ") << ei.line << std::endl
		<< TEXT("Expression: ") << ei.expression << std::endl;

	return ss.str();
}

void Abort(const ErrorInfo& ei)
{
	if(OnError(ei))
		return;

	tstringstream ss;
	ss << FormatError(ei);

	if(!OnErrorOutput(ss.str()))
		OnExitProcess(1);
}

void AbortCLib(const ErrorInfo& ei)
{
	if(OnError(ei))
		return;

	int errcode = std::any_cast<int>(ei.result);

	tstringstream ss;
	const int kMsgBufferSize = 0xff;
	TCHAR msg_buffer[kMsgBufferSize];

	ss << FormatError(ei);

	if(errcode && !_tcserror_s(msg_buffer, kMsgBufferSize, errcode)) {
		ss
			<< TEXT("CLib error code: ") << errcode << std::endl
			<< TEXT("CLib error message: ") << msg_buffer << std::endl;
	}

	if(!OnErrorOutput(ss.str()))
		OnExitProcess(errcode ? errcode : 1);
}

void AbortStdErrorCode(const ErrorInfo& ei)
{
	if(OnError(ei))
		return;

	auto error_code = std::any_cast<std::error_code>(ei.result);

	tstringstream ss;

	ss
		<< FormatError(ei)
		<< TEXT("std::error_code::value: ") << error_code.value() << std::endl
		<< TEXT("std::error_code::message: ") << error_code.message().c_str() << std::endl;

	if(!OnErrorOutput(ss.str()))
		OnExitProcess(error_code.value());
}

void AbortWin32(const ErrorInfo& ei)
{
	if(OnError(ei))
		return;

	tstringstream ss;
	DWORD last_error = GetLastError();
	LPTSTR msg_buffer;
	DWORD size_in_tchar;

	ss << FormatError(ei);

	if(last_error && (size_in_tchar = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, last_error, 0, reinterpret_cast<LPTSTR>(&msg_buffer), 0, 0))) {
		ss
			<< TEXT("Win32 error code: ") << last_error << std::endl
			<< TEXT("Win32 error message: ") << msg_buffer << std::endl;
		LocalFree(msg_buffer);
	}

	if(!OnErrorOutput(ss.str()))
		OnExitProcess(last_error ? last_error : 1);
}

}
