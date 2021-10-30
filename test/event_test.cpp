#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <shoujin/event.hpp>

using namespace shoujin;

TEST_CLASS(EventTest) {
	struct UserData {
		int x, y;
		int add_result, mul_result;
	};

	static void OnEventTwoParam(int x, int y, int& out_result, void* userdata)
	{
		out_result = x + y;
	}

	static void OnEventUserData(void* userdata)
	{
		UserData* ud = reinterpret_cast<UserData*>(userdata);
		ud->add_result = ud->x + ud->y;
		ud->mul_result = ud->x * ud->y;
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
		Event<int, int, int&> event_two_param(OnEventTwoParam);
		int x{3}, y{2}, sum;

		//Act
		event_two_param(x, y, sum);

		//Assert
		Assert::AreEqual(x + y, sum);
	}

	TEST_METHOD(Event_CopyAssignment_EventRaised) {
		//Arrange
		Event<int, int, int&> event_two_param;
		int x{3}, y{2}, sum;

		event_two_param = {OnEventTwoParam};

		//Act
		event_two_param(x, y, sum);

		//Assert
		Assert::AreEqual(x + y, sum);
	}

	TEST_METHOD(Event_OperatorBool_EventRaised) {
		//Arrange
		Event<int, int, int&> event_two_param;

		//Act
		bool before = event_two_param;
		event_two_param = {OnEventTwoParam};
		bool after = event_two_param;

		//Assert
		Assert::IsFalse(before);
		Assert::IsTrue(after);
	}

	TEST_METHOD(Event_UserData_UserDataUpdated) {
		//Arrange
		int x{5}, y{3};
		UserData userdata{x, y};
		Event<> event = {OnEventUserData, &userdata};

		//Act
		event();

		//Assert
		Assert::AreEqual(x + y, userdata.add_result);
		Assert::AreEqual(x * y, userdata.mul_result);
	}
};
