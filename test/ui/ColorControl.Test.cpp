#include <ui/ui.hpp>
#include <vector>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ColorControlTest) {
public:
	TEST_METHOD(ColorControlTest_IsNotCopyConstructible) {
		Assert::IsFalse(std::is_copy_constructible_v<ColorControl>);
	}

	TEST_METHOD(ColorControlTest_IsNotCopyAssignable) {
		Assert::IsFalse(std::is_copy_assignable_v<ColorControl>);
	}

	TEST_METHOD(ColorControlTest_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<ColorControl>);
	}

	TEST_METHOD(ColorControlTest_IsNotMoveAssignable) {
		Assert::IsFalse(std::is_move_assignable_v<ColorControl>);
	}

	TEST_METHOD(ColorControlTest_ShowFourTogether) {
		constexpr int WCX = 960;
		constexpr int WCY = 720;
		constexpr int SPACE = 10;

		Control::ControlCreateInfo cci{
			.client_size = {WCX, WCY},
			.layout = Window::Layout::CenterParent,
			.style = Window::DEFAULT_STYLE,
			.text = TEXT("ColorControlTest_ShowFourTogether")};

		Control parent(cci);

		std::vector<Control*> ctrlv;
		ctrlv.reserve(4);

		auto ws = [](int s) { return s / 2 - (SPACE + (SPACE >> 1)); };

		UserControlCreateInfo ucci{
			.position{SPACE, SPACE},
			.window_size{ws(WCX), ws(WCY)}};

		ctrlv.push_back(parent.AddChild(new ColorControl(ucci)));

		ucci.position.x = WCX - ucci.window_size.cx - SPACE;
		ctrlv.push_back(parent.AddChild(new ColorControl(ucci)));
		
		ucci.position.x = SPACE;
		ucci.position.y = WCY - ucci.window_size.cy - SPACE;
		ctrlv.push_back(parent.AddChild(new ColorControl(ucci)));

		ucci.position.x = WCX - ucci.window_size.cx - SPACE;
		ctrlv.push_back(parent.AddChild(new ColorControl(ucci)));

		parent.Show();
		while(parent.MessageUpdate()) {
			for(auto c : ctrlv)
				c->MessageUpdate();
			Sleep(1);
		}
	}
};
