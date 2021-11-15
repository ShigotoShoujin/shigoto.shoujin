#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;

TEST_CLASS(WindowTabOrderTest) {
public:
	TEST_METHOD(WindowTabOrder_IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<WindowTabOrder>);
	}

	TEST_METHOD(WindowTabOrder_IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<WindowTabOrder>);
	}

	TEST_METHOD(WindowTabOrder_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<WindowTabOrder>);
	}

	TEST_METHOD(WindowTabOrder_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<WindowTabOrder>);
	}
};
