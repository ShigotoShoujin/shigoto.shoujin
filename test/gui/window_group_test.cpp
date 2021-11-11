#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;

TEST_CLASS(WindowGroupTest) {
public:
	TEST_METHOD(WindowGroup_IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<WindowGroup>);
	}

	TEST_METHOD(WindowGroup_IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<WindowGroup>);
	}

	TEST_METHOD(WindowGroup_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<WindowGroup>);
	}

	TEST_METHOD(WindowGroup_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<WindowGroup>);
	}
};
