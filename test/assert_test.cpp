#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shoujin/assert.hpp>

using namespace shoujin;
using namespace shoujin::assert;

TEST_CLASS(AssertTest) {
	static int _exit_process_call_count;
	static tstring _error_message;

public:
	static void ExitProcess(UINT exit_code, tstring error_message)
	{
		++_exit_process_call_count;
	}

	TEST_METHOD(Assert_Test) {
	}
};

int AssertTest::_exit_process_call_count = 0;
tstring AssertTest::_error_message;
