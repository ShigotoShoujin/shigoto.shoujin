#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <shoujin/event.hpp>

using namespace shoujin;

TEST_CLASS(EventTest) {
	static void OnEventTwoParam(int x, int y, int& out_result)
	{
		out_result = x + y;
	}

public:
	TEST_METHOD(Event_IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Event<void*>>);
	}

	TEST_METHOD(Event_IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Event<void*>>);
	}

	TEST_METHOD(Event_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Event<void*>>);
	}

	TEST_METHOD(Event_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Event<void*>>);
	}

	TEST_METHOD(Event_WhenTwoParam_EventRaised) {
		//Arrange
		Event<int, int, int&> Event_two_param(OnEventTwoParam);
		int x{3}, y{2}, sum;

		//Act
		Event_two_param(x, y, sum);

		//Assert
		Assert::AreEqual(x + y, sum);
	}

	TEST_METHOD(Event_CopyAssignment_EventRaised) {
		//Arrange
		Event<int, int, int&> Event_two_param;
		int x{3}, y{2}, sum;

		Event_two_param = {OnEventTwoParam};

		//Act
		Event_two_param(x, y, sum);

		//Assert
		Assert::AreEqual(x + y, sum);
	}

	TEST_METHOD(Event_OperatorBool_EventRaised) {
		//Arrange
		Event<int, int, int&> Event_two_param;

		//Act
		bool before = Event_two_param;
		Event_two_param = {OnEventTwoParam};
		bool after = Event_two_param;

		//Assert
		Assert::IsFalse(before);
		Assert::IsTrue(after);
	}
};
