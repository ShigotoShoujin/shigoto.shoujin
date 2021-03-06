#include "assert.hpp"
#include "stream_to_file_redirector.hpp"
#include <stdio.h>
#include <tchar.h>

namespace shoujin::assert {

StreamToFileRedirector::StreamToFileRedirector(StreamToFileRedirector&& rhs) noexcept :
	_target_stream(rhs._target_stream)
{
	rhs._target_stream = nullptr;
}

StreamToFileRedirector& StreamToFileRedirector::operator=(StreamToFileRedirector&& rhs) noexcept
{
	if(this != &rhs) {
		_target_stream = rhs._target_stream;
		rhs._target_stream = nullptr;
	}
	return *this;
}

StreamToFileRedirector::StreamToFileRedirector(FILE* stream_to_redirect, LPCWSTR output_file) :
	_target_stream{}
{
	SHOUJIN_ASSERT_CLIB(_wfreopen_s(&_target_stream, output_file, L"a", stream_to_redirect));
}

StreamToFileRedirector::~StreamToFileRedirector()
{
	if(_target_stream) {
		fclose(_target_stream);
		_target_stream = nullptr;
	}
}

}
