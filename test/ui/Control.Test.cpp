#include <ui/ui.hpp>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ControlTest) {
public:
	TEST_METHOD(ControlTest_IsNotCopyConstructible) {
		Assert::IsFalse(std::is_copy_constructible_v<Control>);
	}

	TEST_METHOD(ControlTest_IsNotCopyAssignable) {
		Assert::IsFalse(std::is_copy_assignable_v<Control>);
	}

	TEST_METHOD(ControlTest_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Control>);
	}

	TEST_METHOD(ControlTest_IsMoveAssignable) {
		Assert::IsFalse(std::is_move_assignable_v<Control>);
	}

	TEST_METHOD(ControlTest_Test) {
		Control::ControlCreateInfo cci{
			.client_size = {640, 480},
			.layout = Window::Layout::CenterParent,
			.text = TEXT("ControlTest_Test")};

		Control control(cci);

		UserControlCreateInfo ucci{
			.position{10, 10},
			.text{TEXT("New Edit Control")},
			.window_size{200, 20},
		};

		auto add = [&]() {
			control.AddChild(EditControl{ucci});
			ucci.position.y += 30;
		};

		for(auto i = 0; i < 5; ++i)
			add();

		control.Show();
		control.MessageLoop();
	}
};
