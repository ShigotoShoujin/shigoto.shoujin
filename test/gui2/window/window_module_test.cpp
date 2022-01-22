import Shoujin.Gui.Window;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../cppunittest_util.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace shoujin::gui2;

namespace shoujin::test::gui2 {

TEST_CLASS(WindowTest) {
public:
	TEST_METHOD_INITIALIZE(testInitialize)
	{
	}

	TEST_METHOD_CLEANUP(testCleanup)
	{
	}

	TEST_METHOD(constructor_StyleSpecified_WindowHasStyle) {
		WindowStyle expected = WindowStyle::Caption | WindowStyle::MinimizeBox | WindowStyle::SystemMenu;
		Window wnd{WindowCreateInfo{.style = expected}};
		Assert::AreEqual(expected, wnd.style());
	}

	TEST_METHOD(constructor_ClientSizeAndWindowSizeSpecified_WindowSizeHasPriority) {
		Window wnd{WindowCreateInfo{.clientSize{640, 480}, .windowSize{800, 600}}};

		auto [style, exstyle] = styleToNative(wnd.style());
		auto expectedClientSize = layout::getClientSizeFromWindowSize({800, 600}, style, exstyle);

		Assert::AreEqual<Size>(expectedClientSize, wnd.clientSize());
		Assert::AreEqual<Size>({800, 600}, wnd.windowSize());
	}

	TEST_METHOD(constructor_WindowSizeSpecified_ClientSizeAdjusted) {
		Window wnd{WindowCreateInfo{.windowSize{800, 600}}};

		auto [style, exstyle] = styleToNative(wnd.style());
		auto expectedClientSize = layout::getClientSizeFromWindowSize({800, 600}, style, exstyle);

		Assert::AreEqual<Size>(expectedClientSize, wnd.clientSize());
	}

	TEST_METHOD(constructor_WindowSizeSpecified_WindowSizeMatch) {
		Window wnd{WindowCreateInfo{.windowSize{800, 600}}};
		Assert::AreEqual<Size>(Size{800, 600}, wnd.windowSize());
	}

	TEST_METHOD(constructor_ClientSizeSpecified_WindowSizeAdjusted) {
		Window wnd{WindowCreateInfo{.clientSize{800, 600}}};

		auto [style, exstyle] = styleToNative(wnd.style());
		auto expectedWindowSize = layout::getWindowSizeFromClientSize({800, 600}, style, exstyle);

		Assert::AreEqual<Size>(expectedWindowSize, wnd.windowSize());
	}

	TEST_METHOD(constructor_ClientSizeSpecified_ClientSizeMatch) {
		Window wnd{WindowCreateInfo{.clientSize{800, 600}}};
		Assert::AreEqual<Size>(Size{800, 600}, wnd.clientSize());
	}

	TEST_METHOD(constructor_TextIsSpecified_SameTextIsReturned) {
		Window wnd{WindowCreateInfo{.text = L"Greetings"}};
		String expected = L"Greetings";
		Assert::AreEqual<StringView>(expected, wnd.text());
	}

	TEST_METHOD(defaultConstructor_TextIsEmpty) {
		Window wnd;
		String expected = "";
		Assert::AreEqual<String>(expected.c_str(), wnd.text().data());
	}

	TEST_METHOD(defaultConstructor_ClientSizeIsOneFourthOfParentSize) {
		Window wnd;
		auto expected = layout::getParentSize() / 4;
		Assert::AreEqual<Size>(expected, wnd.clientSize());
	}

	TEST_METHOD(defaultConstructor_WindowSizeIsAdjusted) {
		Window wnd;
		auto [style, exstyle] = styleToNative(wnd.style());
		auto expected = layout::getWindowSizeFromClientSize(wnd.clientSize(), style, exstyle);
		Assert::AreEqual<Size>(expected, wnd.windowSize());
	}

	TEST_METHOD(defaultConstructor_IsCentered) {
		Window wnd;
		auto expected = layout::getCenteredPosition(wnd.windowSize());
		Assert::AreEqual(expected, wnd.position());
	}

	TEST_METHOD(defaultConstructor_StyleIsDefault) {
		Window wnd;

		auto expected =
			WindowStyle::Border |
			WindowStyle::Caption |
			WindowStyle::MaximizeBox |
			WindowStyle::MinimizeBox |
			WindowStyle::Sizable |
			WindowStyle::SystemMenu;

		Assert::AreEqual(expected, wnd.style());
	}

	TEST_METHOD(windowGoesOutOfScope_Destroyed) {
		HWND hwnd;

		{
			Window wnd;
			wnd.show();
			hwnd = wnd.handle();
		}

		Assert::IsFalse(IsWindow(hwnd));
	}
};

}
