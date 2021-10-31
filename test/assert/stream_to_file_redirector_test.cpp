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

TEST_CLASS(StreamToFileRedirectorTest) {
	int _exit_process_call_count;

public:
	static void OnExitProcess(bool& cancel, void* userdata)
	{
		int* exit_process_call_count = reinterpret_cast<int*>(userdata);
		cancel = true;
		++*exit_process_call_count;
	}

	TEST_CLASS_INITIALIZE(Initialize){
		shoujin::assert::OnExitProcessEvent = OnExitProcess;
	}

	TEST_CLASS_CLEANUP(TestClassCleanup)
	{
		shoujin::assert::OnExitProcessEvent = nullptr;
	}

	TEST_METHOD(StreamToFileRedirector_IsNotCopyConstructible) {
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

	TEST_METHOD(StreamToFileRedirector_ErrorOutputRedirectedToFile) {
		//Arrange
		const TCHAR* ERR_FILE = TEXT("err_file.txt");
		auto temp_path = std::filesystem::temp_directory_path();
		auto err_file_path = temp_path.replace_filename(ERR_FILE);
		std::filesystem::remove(err_file_path);

		StreamToFileRedirector redirector(stderr, err_file_path.c_str());
		_exit_process_call_count = 0;

		shoujin::assert::OnExitProcessEvent = {OnExitProcess, &_exit_process_call_count};

		//Act
		SHOUJIN_ASSERT_WIN32(CreateWindowEx(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));

		//Assert
		Assert::AreEqual(1, _exit_process_call_count);
 		Assert::IsTrue(FileExists(err_file_path));
	}
};