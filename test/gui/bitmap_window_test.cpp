#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;

static bool OnCreatePostCloseMsg(Window const& window, CREATESTRUCT const& createparam, void* userdata);

TEST_CLASS(BitmapWindowTest) {
public:
	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<BitmapWindowTest>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<BitmapWindowTest>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<BitmapWindowTest>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<BitmapWindowTest>);
	}

	TEST_METHOD(ShowWindow_Paint_OK) {
		BitmapWindow bitmap_window;
		//bitmap_window.OnCreateEvent = OnCreatePostCloseMsg;
		bitmap_window.ShowModal();
	}
};

static bool OnCreatePostCloseMsg(Window const& window, CREATESTRUCT const& createparam, void* userdata)
{
	SHOUJIN_ASSERT(window.handle());
	PostMessage(window.handle()->hwnd(), WM_CLOSE, 0, 0);
	return false;
}
