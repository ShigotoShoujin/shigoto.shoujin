#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <shoujin/event.hpp>

using namespace shoujin;

TEST_CLASS(UserDataEventTest) {
	static void OnUserDataEventTwoParam(int x, int y, int* value)
	{
		*value = x + y;
	}

public:
	TEST_METHOD(UserDataEvent_IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<UserDataEvent<void*>>);
	}

	TEST_METHOD(UserDataEvent_IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<UserDataEvent<void*>>);
	}

	TEST_METHOD(UserDataEvent_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<UserDataEvent<void*>>);
	}

	TEST_METHOD(UserDataEvent_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<UserDataEvent<void*>>);
	}

	TEST_METHOD(UserDataEvent_WhenTwoParam_UserDataEventRaised) {
		//Arrange
		int value;
		UserDataEvent<int*, int, int> UserDataEvent_two_param(OnUserDataEventTwoParam, &value);
		int x{3}, y{2};

		//Act
		UserDataEvent_two_param(x, y);

		//Assert
		Assert::AreEqual(x + y, value);
	}

	TEST_METHOD(UserDataEvent_CopyAssignment_UserDataEventRaised) {
		//Arrange
		int value;
		UserDataEvent<int*, int, int> UserDataEvent_two_param;
		int x{3}, y{2};

		UserDataEvent_two_param = {OnUserDataEventTwoParam, &value};

		//Act
		UserDataEvent_two_param(x, y);

		//Assert
		Assert::AreEqual(x + y, value);
	}

	TEST_METHOD(UserDataEvent_OperatorBool_UserDataEventRaised) {
		//Arrange
		int value;
		UserDataEvent<int*, int, int> UserDataEvent_two_param;

		//Act
		bool before = UserDataEvent_two_param;
		UserDataEvent_two_param = {OnUserDataEventTwoParam, &value};
		bool after = UserDataEvent_two_param;

		//Assert
		Assert::IsFalse(before);
		Assert::IsTrue(after);
	}
};
