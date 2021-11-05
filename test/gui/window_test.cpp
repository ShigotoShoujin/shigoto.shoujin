#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/gui.hpp>
#include <shoujin/assert.hpp>

using namespace shoujin::gui;

//class ColorPicker : public Window {
//	Bitmap _bitmap;
//
//public:
//	ColorPicker(Point position, Size window_size) :
//		Window{
//			CreateInfo{
//				.position{position},
//				.window_size{window_size},
//				.style{WS_CHILD},
//				.exstyle{WS_EX_STATICEDGE},
//			},
//		},
//		_bitmap{client_size()}
//	{
//		_bitmap.Fill(Color::Navy);
//	}
//
//	virtual bool OnWndProc(const WindowMessage& message) override
//	{
//		switch(message.msg) {
//			case WM_PAINT:
//				Window::ProcessOnPaintMessageFromDC(_bitmap.hdc());
//				return false;
//			case WM_SIZE:
//				return false;
//		}
//
//		return Window::OnWndProc(message);
//	}
//};

TEST_CLASS(WindowTest) {
public:
	TEST_CLASS_INITIALIZE(ClassInitialize)
	{
		shoujin::assert::_display_error_messagebox_ = true;
	}

	TEST_METHOD(Window_IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Window>);
	}

	TEST_METHOD(Window_IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Window>);
	}

	TEST_METHOD(Window_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Window>);
	}

	TEST_METHOD(Window_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Window>);
	}

	TEST_METHOD(Window_WIP) {
		Window window{Window::CreateInfo{.create_mode = Window::CreateMode::Centered, .style = Window::DefaultStyle | WS_SIZEBOX}};

		//ColorPicker cp({}, window.client_size() / 2, window.hwnd());

		window.ShowModal();
	}
};
