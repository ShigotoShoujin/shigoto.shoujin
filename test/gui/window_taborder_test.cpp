#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;

TEST_CLASS(WindowTabOrderTest) {
public:
	TEST_METHOD(IsCopyConstructible) {
		static_assert(std::is_copy_constructible_v<WindowTabOrder>);
	}

	TEST_METHOD(IsCopyAssignable) {
		static_assert(std::is_copy_assignable_v<WindowTabOrder>);
	}

	TEST_METHOD(IsMoveConstructible) {
		static_assert(std::is_move_constructible_v<WindowTabOrder>);
	}

	TEST_METHOD(IsMoveAssignable) {
		static_assert(std::is_move_assignable_v<WindowTabOrder>);
	}
};
