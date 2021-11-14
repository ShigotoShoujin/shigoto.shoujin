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
		shoujin::gui::logging::_activate_wndproc_messagelog_ = true;
	}

	TEST_METHOD_CLEANUP(TestCleanup)
	{
		shoujin::gui::logging::_activate_wndproc_messagelog_ = false;
	}

	TEST_METHOD(ColorControl_IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<ColorControl>);
	}

	TEST_METHOD(ColorControl_IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<ColorControl>);
	}

	TEST_METHOD(ColorControl_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<ColorControl>);
	}

	TEST_METHOD(ColorControl_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<ColorControl>);
	}

	TEST_METHOD(ColorControl_WIP_Test) {
		ColorControl cc{LayoutParam{.layout_mode{LayoutMode::CenterParent}}};
		cc.ShowModal();
	}
};