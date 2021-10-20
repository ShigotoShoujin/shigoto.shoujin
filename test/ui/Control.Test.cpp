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
		const int WCX = 640;
		const int WCY = 480;
		const int SPACE = 10;

		Control::ControlCreateInfo cci{
			.client_size = {WCX, WCY},
			.layout = Window::Layout::CenterParent,
			.text = TEXT("ControlTest_Test")};

		Control control(cci);

		UserControlCreateInfo ucci{
			.position{SPACE, SPACE},
			.text{TEXT("New Edit Control")},
			.window_size{200, 20},
		};

		auto addEditControl = [&]() {
			control.AddChild(EditControl{ucci});
			ucci.position.y += 20 + SPACE;
		};

		for(auto i = 0; i < 5; ++i)
			addEditControl();

		ucci.position = {WCX - SPACE - SPACE - 96 - 96, WCY - SPACE - 24};
		ucci.client_size = {};
		ucci.window_size = {96, 24};
		ucci.text = {TEXT("OK")};
		control.AddChild(ButtonControl{ucci});

		ucci.position.x -= -SPACE - 96;
		ucci.text = {TEXT("Close")};
		control.AddChild(ButtonControl{ucci});

		control.Show();
		control.MessageLoop();
	}
};
