#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <shoujin/event.hpp>

using namespace shoujin;

TEST_CLASS(EventTest) {
	static void OnEventTwoParam(int x, int y, int& value)
	{
		value = x + y;
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
		int value;
		Event<int, int, int> event_two_param(OnEventTwoParam, value);
		int x{3}, y{2};

		//Act
		event_two_param(x, y);

		//Assert
		Assert::AreEqual(x + y, value);
	}

	TEST_METHOD(Event_CopyAssignment_EventRaised) {
		//Arrange
		int value;
		Event<int, int, int> event_two_param;
		int x{3}, y{2};

		event_two_param = {OnEventTwoParam, value};

		//Act
		event_two_param(x, y);

		//Assert
		Assert::AreEqual(x + y, value);
	}

	TEST_METHOD(Event_OperatorBool_EventRaised) {
		//Arrange
		int value;
		Event<int, int, int> event_two_param;
		int x{3}, y{2};

		//Act
		bool before = event_two_param;
		event_two_param = {OnEventTwoParam, value};
		bool after = event_two_param;

		//Assert
		Assert::IsFalse(before);
		Assert::IsTrue(after);
	}
};
