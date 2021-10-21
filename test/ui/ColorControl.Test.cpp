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

	TEST_METHOD(ColorControlTest_IsMoveAssignable) {
		Assert::IsFalse(std::is_move_assignable_v<ColorControl>);
	}

	TEST_METHOD(ColorControlTest_ShowFourTogether) {
		constexpr int WCX = 960;
		constexpr int WCY = 720;
		constexpr int SPACE = 10;

		Control::ControlCreateInfo cci{
			.client_size = {WCX, WCY},
			.layout = Window::Layout::CenterParent,
			.text = TEXT("ColorControlTest_ShowFourTogether")};

		Control parent(cci);

		std::vector<Control*> ctrlv;
		ctrlv.reserve(4);

		auto ws = [](int s) { return s / 2 - (SPACE + (SPACE >> 1)); };

		UserControlCreateInfo ucci{
			.position{SPACE, SPACE},
			.window_size{ws(WCX), ws(WCY)}};

		ctrlv.push_back(parent.AddChild(ColorControl(ucci)));

		//ucci.position.x = WCX - ucci.window_size.cx - SPACE;
		//ctrlv.push_back(parent.AddChild(ColorControl(ucci)));

		//ucci.position.x = SPACE;
		//ucci.position.y = WCY - ucci.window_size.cy - SPACE;
		//ctrlv.push_back(parent.AddChild(ColorControl(ucci)));

		//ucci.position.x = WCX - ucci.window_size.cx - SPACE;
		//ctrlv.push_back(parent.AddChild(ColorControl(ucci)));

		Window::WindowCreateInfo wci_center
		{
			.ex_style = WS_EX_CLIENTEDGE,
			.position{ 50, 350},
			.window_size = {WCX / 5, WCY / 5}};

		//SetParent needs to redo CenterParent with new window, same for Fill, just recreate it instead, well just create it first time in set parent instead of in ctor ffs

		Window center_wnd(wci_center);

		HWND hwnd = center_wnd.GetHandle();
		SetWindowLongPtr(hwnd, GWL_STYLE, WS_CHILD);
		::SetParent(hwnd, parent.GetHandle());
		SetWindowPos(hwnd, 0, 0, 0, wci_center.window_size.cx, wci_center.window_size.cy, SWP_NOZORDER | SWP_NOMOVE);
		center_wnd.Show();

		parent.Show();
		while(parent.MessageUpdate()) {
			center_wnd.MessageUpdate();
			for(auto c : ctrlv)
				c->MessageUpdate();
		}
	}
};
