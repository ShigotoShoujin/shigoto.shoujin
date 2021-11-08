#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin::gui;

TEST_CLASS(WindowTest) {
public:
	TEST_METHOD(Window_IsNotCopyConstructible) {
		Assert::IsFalse(std::is_copy_constructible_v<Window>);
	}

	TEST_METHOD(Window_IsNotCopyAssignable) {
		Assert::IsFalse(std::is_copy_assignable_v<Window>);
	}

	TEST_METHOD(Window_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Window>);
	}

	TEST_METHOD(Window_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Window>);
	}

	TEST_METHOD(Window_CreateAndClose_NoAssertions) {
		Window window{Window::LayoutInfo{.create_mode = Window::CreateMode::CenterParent, .style = Window::DefaultStyle | WS_SIZEBOX}};
		window.Show();
		window.Close();
	}
};
