#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/gui.hpp>

using namespace shoujin::gui;

TEST_CLASS(WindowCoreTest) {
public:
	TEST_METHOD(WindowCore_IsNotCopyConstructible) {
		Assert::IsFalse(std::is_copy_constructible_v<WindowCore>);
	}

	TEST_METHOD(WindowCore_IsNotCopyAssignable) {
		Assert::IsFalse(std::is_copy_assignable_v<WindowCore>);
	}

	TEST_METHOD(WindowCore_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<WindowCore>);
	}

	TEST_METHOD(WindowCore_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<WindowCore>);
	}
};
