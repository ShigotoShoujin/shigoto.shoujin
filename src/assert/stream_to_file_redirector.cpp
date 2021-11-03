#include "stream_to_file_redirector.hpp"
#include <stdio.h>
#include <tchar.h>
#include "assert.hpp"

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

StreamToFileRedirector::StreamToFileRedirector(FILE* stream_to_redirect, LPCTSTR output_file)
{
	SHOUJIN_ASSERT_CLIB(_tfreopen_s(&_target_stream, output_file, TEXT("a"), stream_to_redirect));
}

StreamToFileRedirector::~StreamToFileRedirector()
{
	if(_target_stream) {
		fclose(_target_stream);
		_target_stream = nullptr;
	}
}

}
