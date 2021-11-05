#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/gui.hpp>
#include <shoujin/assert.hpp>

using namespace shoujin::gui;

class ColorPicker : public Window {
	std::unique_ptr<Bitmap> _bitmap;

public:
	ColorPicker(Point position, Size window_size) :
		Window{
			CreateInfo{
				.position{position},
				.window_size{window_size},
				.exstyle{WS_EX_STATICEDGE},
			},
		}
	{}

	virtual bool OnCreate(const CREATESTRUCT& createinfo) override
	{
		auto cs = Window::client_size();
		_bitmap.reset(new Bitmap(cs));
		_bitmap->Fill(Color::Cyan);
		_bitmap->Fill(cs / 4, cs / 2, Color::Red);
		return false;
	}

	virtual bool OnPaint() override
	{
		if(_bitmap)
			Window::ProcessOnPaintMessageFromDC(_bitmap->hdc());
		return false;
	}
};

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

		window.AddChild(new ColorPicker({}, window.client_size() / 2));

		window.ShowModal();
	}
};
