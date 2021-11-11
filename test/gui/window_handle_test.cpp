#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/gui.hpp>

using namespace shoujin::gui;

TEST_CLASS(WindowHandleTest) {
public:
	TEST_METHOD(WindowHandle_IsNotCopyConstructible) {
		Assert::IsFalse(std::is_copy_constructible_v<WindowHandle>);
	}

	TEST_METHOD(WindowHandle_IsNotCopyAssignable) {
		Assert::IsFalse(std::is_copy_assignable_v<WindowHandle>);
	}

	TEST_METHOD(WindowHandle_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<WindowHandle>);
	}

	TEST_METHOD(WindowHandle_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<WindowHandle>);
	}
};
