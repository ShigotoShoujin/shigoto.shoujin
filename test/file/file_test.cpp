#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <filesystem>

#include <shoujin/assert.hpp>
#include <shoujin/file.hpp>

using namespace shoujin;
using namespace shoujin::assert;
using namespace shoujin::file;

TEST_CLASS(FileTest) {
public:
	static bool OnExitProcess(void* userdata)
	{
		int* exit_process_call_count = reinterpret_cast<int*>(userdata);
		++*exit_process_call_count;
		return true;
	}

	TEST_CLASS_CLEANUP(TestClassCleanup)
	{
		shoujin::assert::OnExitProcessEvent = nullptr;
	}

	TEST_METHOD(FileDelete_WhenNameMalformed_RaiseExitProcessEvent) {
		//Arrange
		const TCHAR* kBadFileName = TEXT("\\/:*?\"<>|");
		int exit_process_call_count = 0;
		shoujin::assert::OnExitProcessEvent = {OnExitProcess, &exit_process_call_count};

		//Act
		FileDelete(kBadFileName);

		//Assert
		Assert::AreEqual(1, exit_process_call_count);
	}
};
