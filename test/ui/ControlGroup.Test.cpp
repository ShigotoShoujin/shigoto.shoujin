#include "ui/ControlGroup.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ControlGroupTest) {
public:
	TEST_METHOD(ControlGroupTest_IsNotCopyConstructible) {
		Assert::IsFalse(std::is_copy_constructible_v<ControlGroup>);
	}

	TEST_METHOD(ControlGroupTest_IsNotCopyAssignable) {
		Assert::IsFalse(std::is_copy_assignable_v<ControlGroup>);
	}

	TEST_METHOD(ControlGroupTest_IsNotMoveConstructible) {
		Assert::IsFalse(std::is_move_constructible_v<ControlGroup>);
	}

	TEST_METHOD(ControlGroupTest_IsNotMoveAssignable) {
		Assert::IsFalse(std::is_move_assignable_v<ControlGroup>);
	}
};
