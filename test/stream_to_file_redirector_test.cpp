#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>

#include <shoujin/assert.hpp>

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

namespace shoujin::assert {
//TODO add redirection to console, see CONOUT$

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
	ASSERT_CLIB(0, _tfreopen_s(&_target_stream, output_file, TEXT("a"), stream_to_redirect));
}

StreamToFileRedirector::~StreamToFileRedirector()
{
	if(_target_stream) {
		fclose(_target_stream);
		_target_stream = nullptr;
	}
}
}

using namespace shoujin::assert;

TEST_CLASS(StreamToFileRedirectorTest) {
	static int _exit_process_call_count;

public:
	static void ExitProcess(UINT exit_code)
	{
		++_exit_process_call_count;
	}

	TEST_CLASS_INITIALIZE(Initialize){
		shoujin::assert::ExitProcessFunc = ExitProcess;
	}

	TEST_METHOD(StreamToFileRedirector_IsNotConstructible) {
		Assert::IsFalse(std::is_copy_constructible_v<StreamToFileRedirector>);
	}

	TEST_METHOD(StreamToFileRedirector_IsNotCopyAssignable) {
		Assert::IsFalse(std::is_copy_assignable_v<StreamToFileRedirector>);
	}

	TEST_METHOD(StreamToFileRedirector_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<StreamToFileRedirector>);
	}

	TEST_METHOD(StreamToFileRedirector_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<StreamToFileRedirector>);
	}

	TEST_METHOD(StreamToFileRedirector_Test) {
		//Arrange
		_exit_process_call_count = 0;

		StreamToFileRedirector redirector(stderr, TEXT("err_file.txt"));

		//Act
		ASSERT_WIN32(CreateWindowEx(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));

		//Assert
		Assert::AreEqual(1, _exit_process_call_count);
	}
};

int StreamToFileRedirectorTest::_exit_process_call_count = 0;
