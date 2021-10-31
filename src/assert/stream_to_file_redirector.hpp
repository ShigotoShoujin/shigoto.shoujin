#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

//TODO add redirection to console, see CONOUT$

namespace shoujin::assert {
class StreamToFileRedirector {
public:
	StreamToFileRedirector(const StreamToFileRedirector&) = delete;
	StreamToFileRedirector& operator=(const StreamToFileRedirector&) = delete;
	StreamToFileRedirector(StreamToFileRedirector&&) noexcept;
	StreamToFileRedirector& operator=(StreamToFileRedirector&&) noexcept;

	StreamToFileRedirector(FILE* stream_to_redirect, LPCTSTR output_file);
	~StreamToFileRedirector();

private:
	FILE* _target_stream;
};
}