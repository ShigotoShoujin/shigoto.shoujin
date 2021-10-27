#include <ui/ui.hpp>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(BitmapControlTest) {
public:
	TEST_METHOD(BitmapControlTest_IsNotCopyConstructible) {
		Assert::IsFalse(std::is_copy_constructible_v<BitmapControl>);
	}

	TEST_METHOD(BitmapControlTest_IsNotCopyAssignable) {
		Assert::IsFalse(std::is_copy_assignable_v<BitmapControl>);
	}

	TEST_METHOD(BitmapControlTest_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<BitmapControl>);
	}

	TEST_METHOD(BitmapControlTest_IsNotMoveAssignable) {
		Assert::IsFalse(std::is_move_assignable_v<BitmapControl>);
	}
};
