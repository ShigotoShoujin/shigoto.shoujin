#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;
using namespace shoujin::gui::layout;

void AssertEmptyLayout(const LayoutParam& lp);
void AssertLayout(const LayoutParam& expected, const LayoutParam& actual);

TEST_CLASS(LayoutStreamTest) {
	LayoutParam _button{.position{}, .window_size{160, 20}};
	LayoutParam _middle{.position{150, 100}, .window_size{320, 240}};

public:
	TEST_METHOD(IsCopyConstructible) {
		static_assert(std::is_copy_constructible_v<LayoutStream>);
	}

	TEST_METHOD(IsCopyAssignable) {
		static_assert(std::is_copy_assignable_v<LayoutStream>);
	}

	TEST_METHOD(IsMoveConstructible) {
		static_assert(std::is_move_constructible_v<LayoutStream>);
	}

	TEST_METHOD(IsMoveAssignable) {
		static_assert(std::is_move_assignable_v<LayoutStream>);
	}

	TEST_METHOD(GivenNoOps_EmptyLayout) {
		LayoutStream ls;

		LayoutParam actual = ls;

		AssertEmptyLayout(actual);
	}

	TEST_METHOD(GivenOneLayoutOps_SameLayout) {
		LayoutStream ls;

		LayoutParam actual = ls << _middle;

		AssertLayout(_middle, actual);
	}

	TEST_METHOD(GivenOneFromOps_EmptyLayout) {
		LayoutStream ls;

		LayoutParam actual = ls << from(_middle);

		AssertEmptyLayout(actual);
	}

	TEST_METHOD(GivenOneAboveOps_EmptyLayout) {
		LayoutStream ls;
		LayoutParam expected{.position{0, -7}};

		LayoutParam actual = ls << above;

		AssertLayout(expected, actual);
	}

	TEST_METHOD(GivenFromMiddleOneButton_WhenAbove_LayoutAbove) {
		LayoutStream ls;
		LayoutParam expected{.position{150, 100 - 20 - 7}, .window_size{160, 20}};

		LayoutParam actual = ls << from(_middle) << _button << above;

		AssertLayout(expected, actual);
	}

	TEST_METHOD(GivenFromMiddleOneButton_WhenAfter_LayoutAfter) {
		LayoutStream ls;
		LayoutParam expected{.position{150 + 320 + 7, 100}, .window_size{_button.window_size}};

		LayoutParam actual = ls << from(_middle) << _button << after;

		AssertLayout(expected, actual);
	}

	TEST_METHOD(GivenFromMiddleOneButton_WhenBefore_LayoutBefore) {
		LayoutStream ls;
		LayoutParam expected{.position{150 - 160 - 7, 100}, .window_size{_button.window_size}};

		LayoutParam actual = ls << from(_middle) << _button << before;

		AssertLayout(expected, actual);
	}

	TEST_METHOD(GivenFromMiddleOneButton_WhenBelow_LayoutBelow) {
		LayoutStream ls;
		LayoutParam expected{.position{150, 100 + 240 + 7}, .window_size{_button.window_size}};

		LayoutParam actual = ls << from(_middle) << _button << below;

		AssertLayout(expected, actual);
	}
};

void AssertEmptyLayout(const LayoutParam& lp)
{
	DWORD nostyle{};
	//Assert::AreEqual(LayoutMode::Default, lp.layout_mode);
	Assert::AreEqual(0, lp.position.x);
	Assert::AreEqual(0, lp.position.y);
	Assert::AreEqual(0, lp.window_size.x);
	Assert::AreEqual(0, lp.window_size.y);
	Assert::AreEqual(0, lp.client_size.y);
	Assert::AreEqual(0, lp.client_size.y);
	Assert::AreEqual(nostyle, lp.style);
	Assert::AreEqual(nostyle, lp.exstyle);
}

void AssertLayout(const LayoutParam& expected, const LayoutParam& actual)
{
	//Assert::AreEqual(expected.layout_mode, actual.layout_mode);
	Assert::AreEqual(expected.position.x, actual.position.x);
	Assert::AreEqual(expected.position.y, actual.position.y);
	Assert::AreEqual(expected.window_size.x, actual.window_size.x);
	Assert::AreEqual(expected.window_size.y, actual.window_size.y);
	Assert::AreEqual(expected.client_size.y, actual.client_size.y);
	Assert::AreEqual(expected.client_size.y, actual.client_size.y);
	Assert::AreEqual(expected.style, actual.style);
	Assert::AreEqual(expected.exstyle, actual.exstyle);
}
