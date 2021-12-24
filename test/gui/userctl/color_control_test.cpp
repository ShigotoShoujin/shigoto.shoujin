#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;
using namespace shoujin::gui::comctl32;

TEST_CLASS(ColorControlTest) {
public:
	TEST_METHOD_INITIALIZE(TestInitialize)
	{
		shoujin::assert::_activate_assert_messagebox_ = true;
		shoujin::gui::logging::_activate_wndproc_messagelog_ = false;
	}

	TEST_METHOD_CLEANUP(TestCleanup)
	{
		shoujin::assert::_activate_assert_messagebox_ = false;
		shoujin::gui::logging::_activate_wndproc_messagelog_ = false;
	}

	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<ColorControl>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<ColorControl>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<ColorControl>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<ColorControl>);
	}

	TEST_METHOD(ShowModal_LooksNice_Ok) {
		ColorControl cc{LayoutParam{.layout_mode{LayoutMode::CenterParent}}};
		cc.ShowModal();
	}
};
