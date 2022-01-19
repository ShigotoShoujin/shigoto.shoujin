import Shoujin.Gui.Window;

#include "CppUnitTest.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace shoujin::gui2;

namespace shoujin::test::gui2 {

TEST_CLASS(WindowStyleTest) {
public:
	TEST_METHOD(styleToNative_AllStyles_ConvertAll) {
		WindowStyle windowStyle{
			WindowStyle::None |
			WindowStyle::Border |
			WindowStyle::BorderClientEdge |
			WindowStyle::BorderStaticEdge |
			WindowStyle::BorderWindowEdge |
			WindowStyle::Caption |
			WindowStyle::Disabled |
			WindowStyle::MaximizeBox |
			WindowStyle::MinimizeBox |
			WindowStyle::Sizable |
			WindowStyle::SystemMenu |
			WindowStyle::TopMost};

		auto [style, exStyle] = styleToNative(windowStyle);

		Assert::IsTrue(style & WS_BORDER);
		Assert::IsTrue(style & WS_DLGFRAME);
		Assert::IsTrue(style & WS_DISABLED);
		Assert::IsTrue(style & WS_MAXIMIZEBOX);
		Assert::IsTrue(style & WS_MINIMIZEBOX);
		Assert::IsTrue(style & WS_SIZEBOX);
		Assert::IsTrue(style & WS_SYSMENU);
		Assert::IsTrue(exStyle & WS_EX_CLIENTEDGE);
		Assert::IsTrue(exStyle & WS_EX_STATICEDGE);
		Assert::IsTrue(exStyle & WS_EX_WINDOWEDGE);
		Assert::IsTrue(exStyle & WS_EX_TOPMOST);
	}

	TEST_METHOD(styleFromNative_AllStyles_ConvertAll) {
		DWORD style{
			WS_BORDER |
			WS_DLGFRAME |
			WS_DISABLED |
			WS_MAXIMIZEBOX |
			WS_MINIMIZEBOX |
			WS_SIZEBOX |
			WS_SYSMENU};

		DWORD exStyle{
			WS_EX_CLIENTEDGE |
			WS_EX_STATICEDGE |
			WS_EX_WINDOWEDGE |
			WS_EX_TOPMOST};

		auto windowStyle = styleFromNative(style, exStyle);

		Assert::IsTrue((windowStyle & WindowStyle::Border) == WindowStyle::Border);
		Assert::IsTrue((windowStyle & WindowStyle::Caption) == WindowStyle::Caption);
		Assert::IsTrue((windowStyle & WindowStyle::Disabled) == WindowStyle::Disabled);
		Assert::IsTrue((windowStyle & WindowStyle::MaximizeBox) == WindowStyle::MaximizeBox);
		Assert::IsTrue((windowStyle & WindowStyle::MinimizeBox) == WindowStyle::MinimizeBox);
		Assert::IsTrue((windowStyle & WindowStyle::Sizable) == WindowStyle::Sizable);
		Assert::IsTrue((windowStyle & WindowStyle::SystemMenu) == WindowStyle::SystemMenu);
		Assert::IsTrue((windowStyle & WindowStyle::BorderClientEdge) == WindowStyle::BorderClientEdge);
		Assert::IsTrue((windowStyle & WindowStyle::BorderStaticEdge) == WindowStyle::BorderStaticEdge);
		Assert::IsTrue((windowStyle & WindowStyle::BorderWindowEdge) == WindowStyle::BorderWindowEdge);
		Assert::IsTrue((windowStyle & WindowStyle::TopMost) == WindowStyle::TopMost);
	}
};

}
