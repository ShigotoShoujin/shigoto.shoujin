#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin::gui;

TEST_CLASS(ColorPickerTest) {
public:
	TEST_METHOD_CLEANUP(ClassMethodCleanup)
	{
		shoujin::assert::_display_error_messagebox_ = false;
	}

	TEST_METHOD(ColorPicker_IsNotCopyConstructible) {
		Assert::IsFalse(std::is_copy_constructible_v<ColorPicker>);
	}

	TEST_METHOD(ColorPicker_IsNotCopyAssignable) {
		Assert::IsFalse(std::is_copy_assignable_v<Window>);
	}

	TEST_METHOD(ColorPicker_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Window>);
	}

	TEST_METHOD(ColorPicker_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Window>);
	}

	TEST_METHOD(ColorPicker_WIP) {
		shoujin::assert::_display_error_messagebox_ = true;
		Window window{Window::LayoutInfo{.create_mode = Window::CreateMode::CenterParent, .style = Window::DefaultStyle | WS_SIZEBOX}};

		window.AddChild(new ColorPicker({}, window.client_size() / 2));

		window.ShowModal();
	}
};
