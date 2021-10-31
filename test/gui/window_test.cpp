#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

	TEST_METHOD(Window_WIP) {
		Window window;

		window.Show();

		auto add = [&](int y, int64_t id) {
			CreateWindowEx(WS_EX_STATICEDGE, TEXT("EDIT"), TEXT("Hello World"), WS_CHILD | WS_VISIBLE, 10, y, 128, 24, window.Handle(), reinterpret_cast<HMENU>(id), GetModuleHandle(nullptr), nullptr);
		};

		int y = 10;
		add(y, 1);
		add(y += 34, 2);
		add(y += 34, 3);

		window.ShowModal();
	}
};