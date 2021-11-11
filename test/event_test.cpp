#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/event.hpp>

using namespace shoujin;

TEST_CLASS(EventTest) {
	struct UserData {
		int x, y;
		int add_result, mul_result;
	};

	static void OnVoidEvent(int x, int y, int& out_sum, void* userdata)
	{
		out_sum = x + y;
	}

	static int OnIntEvent(int x, int y, void* userdata)
	{
		return x + y;
	}

	static void OnEventUserData(void* userdata)
	{
		UserData* ud = reinterpret_cast<UserData*>(userdata);
		ud->add_result = ud->x + ud->y;
		ud->mul_result = ud->x * ud->y;
	}

public:
	TEST_METHOD(Event_IsCopyConstructible) {

		Assert::IsTrue(std::is_copy_constructible_v<Event<>>);
	}

	TEST_METHOD(Event_IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Event<>>);
	}

	TEST_METHOD(Event_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Event<>>);
	}

	TEST_METHOD(Event_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Event<>>);
	}

	TEST_METHOD(Event_VoidParam_EventRaised) {
		//Arrange
		Event<void, int, int, int&> void_event(OnVoidEvent);
		int x{3}, y{2}, sum;

		//Act
		void_event(x, y, sum);

		//Assert
		Assert::AreEqual(x + y, sum);
	}

	TEST_METHOD(Event_IntEvent_EventRaised) {
		//Arrange
		Event<int, int, int> int_event(OnIntEvent);
		int x{3}, y{2};

		//Act
		auto sum = int_event(x, y);

		//Assert
		Assert::AreEqual(x + y, sum);
	}

	TEST_METHOD(Event_CopyAssignment_EventRaised) {
		//Arrange
		Event<void, int, int, int&> event_three_param;
		int x{3}, y{2}, sum;

		event_three_param = OnVoidEvent;

		//Act
		event_three_param(x, y, sum);

		//Assert
		Assert::AreEqual(x + y, sum);
	}

	TEST_METHOD(Event_OperatorBool_EventRaised) {
		//Arrange
		Event<void, int, int, int&> event_three_param;

		//Act
		bool before = event_three_param;
		event_three_param = OnVoidEvent;
		bool after = event_three_param;

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
//
//
//
//template<typename TResult = void, typename... TArguments>
//requires fundamental<TResult>
//class Event2 {
//	using TFunc = TResult (*)(TArguments..., void* userdata);
//	TFunc _func;
//	void* _userdata;
//
//public:
//	Event2();
//	Event2(TFunc func, void* userdata = nullptr);
//	Event2& operator=(const Event2& rhs);
//	~Event2();
//
//	TResult operator()(TArguments... args) const;
//	operator bool() const;
//};
//
//template<typename TResult, typename... TArguments>
//requires fundamental<TResult>
//Event2<TResult, TArguments...>::Event2() :
//	_func{},
//	_userdata{}
//{}
//
//template<typename TResult, typename... TArguments>
//Event2<TResult, TArguments...>::Event2(TFunc func, void* userdata) :
//	_func{func},
//	_userdata{userdata}
//{}
//
//template<typename TResult, typename... TArguments>
//Event2<TResult, TArguments...>& Event2<TResult, TArguments...>::operator=(const Event2<TResult, TArguments...>& rhs)
//{
//	_func = rhs._func;
//	_userdata = rhs._userdata;
//	return *this;
//}
//
//template<typename TResult, typename... TArguments>
//Event2<TResult, TArguments...>::~Event2()
//{
//	_func = nullptr;
//	_userdata = nullptr;
//}
//
//template<typename TResult, typename... TArguments>
//TResult Event2<TResult, TArguments...>::operator()(TArguments... args) const
//{
//	if(_func)
//		if constexpr(std::is_void_v<TResult>)
//			_func(args..., _userdata);
//		else
//			return _func(args..., _userdata);
//	else if constexpr(!std::is_void_v<TResult>)
//		return TResult{};
//}
//
//template<typename TResult, typename... TArguments>
//Event2<TResult, TArguments...>::operator bool() const
//{
//	return _func != nullptr;
//}
//
//void Test()
//{
//	//Event2<> e2;
//	//Event2<void, int> e3;
//	//Event2<bool, int> e4;
//}
