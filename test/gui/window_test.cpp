#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/gui.hpp>
#include <shoujin/assert.hpp>

using namespace shoujin::gui;

class ColorPicker : public Window {
	Bitmap _bitmap;

public:
	ColorPicker(Point position, Size window_size, HWND hparentwnd) :
		Window{
			WindowLayout{
				CreateInfo{
					.position{position},
					.window_size{window_size},
					.style{WS_CHILD},
					.exstyle{WS_EX_STATICEDGE},
				}},
			hparentwnd},
		_bitmap{WindowLayout::client_size()}
	{
		_bitmap.Fill(Color::Navy);
	}

	virtual bool OnWndProc(const WindowMessage& message) override
	{
		switch(message.msg) {
			case WM_PAINT:
				Window::ProcessOnPaintMessageFromDC(_bitmap.hdc());
				return false;
			case WM_SIZE:
				return false;
		}

		return Window::OnWndProc(message);
	}
};

TEST_CLASS(WindowTest) {
public:
	TEST_CLASS_INITIALIZE(ClassInitialize)
	{
		shoujin::assert::_display_error_messagebox_ = true;
	}

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

	TEST_METHOD(Window_WIP) {
		Window window{WindowLayout{WindowLayout::CreateInfo{.create_mode = WindowLayout::CreateMode::Centered, .style = WindowLayout::DefaultStyle | WS_SIZEBOX}}};

		window.Show();

		//auto add = [&](int y, int64_t id) {
		//	CreateWindowEx(WS_EX_STATICEDGE, TEXT("EDIT"), TEXT("Hello World"), WS_CHILD | WS_VISIBLE, 10, y, 128, 24, window.hwnd(), reinterpret_cast<HMENU>(id), GetModuleHandle(nullptr), nullptr);
		//};
		//
		//int y = 10;
		//add(y, 1);
		//add(y += 34, 2);
		//add(y += 34, 3);

		ColorPicker cp({}, window.client_size() / 2, window.hwnd());

		{
			//Handle the creation differently
			cp.Show();
		}

		window.ShowModal();
	}
};
