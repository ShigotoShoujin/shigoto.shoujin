#include <ui/ui.hpp>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(EditControlTest) {
public:
	TEST_METHOD(EditControlTest_IsNotCopyConstructible) {
		Assert::IsFalse(std::is_copy_constructible_v<EditControl>);
	}

	TEST_METHOD(EditControlTest_IsNotCopyAssignable) {
		Assert::IsFalse(std::is_copy_assignable_v<EditControl>);
	}

	TEST_METHOD(EditControlTest_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<EditControl>);
	}

	TEST_METHOD(EditControlTest_IsMoveAssignable) {
		Assert::IsFalse(std::is_move_assignable_v<EditControl>);
	}
};
