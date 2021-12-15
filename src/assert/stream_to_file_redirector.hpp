#ifndef SHOUJIN_SOURCE_ASSERT_STREAM_TO_FILE_REDIRECTOR
#define SHOUJIN_SOURCE_ASSERT_STREAM_TO_FILE_REDIRECTOR

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

namespace shoujin::assert {

class StreamToFileRedirector {
	FILE* _target_stream;

public:
	StreamToFileRedirector(const StreamToFileRedirector&) = delete;
	StreamToFileRedirector& operator=(const StreamToFileRedirector&) = delete;
	StreamToFileRedirector(StreamToFileRedirector&&) noexcept;
	StreamToFileRedirector& operator=(StreamToFileRedirector&&) noexcept;

	StreamToFileRedirector(FILE* stream_to_redirect, LPCTSTR output_file);
	~StreamToFileRedirector();
};

}

#endif
