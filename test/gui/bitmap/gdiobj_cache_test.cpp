#include "../../../src/gui/bitmap/gdiobj_cache.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::gui;
using namespace shoujin::gui::bitmap;

TEST_CLASS(GdiObjCacheTest) {
public:
	TEST_METHOD(WhenRequestingAYellowBrush_ReturnAYellowBrush) {
		GdiObjCache cache;

		HBRUSH hbrush = cache.GetBrush(Color::Yellow);

		auto actual_color = GetBrushColor(hbrush);
		Assert::AreEqual<COLORREF>(actual_color, Color::Yellow);
	}

	TEST_METHOD(WhenRequestingAYellowBrushTwice_SameBrushIsReturned) {
		GdiObjCache cache;

		HBRUSH hbrush_a = cache.GetBrush(Color::Yellow);
		HBRUSH hbrush_b = cache.GetBrush(Color::Yellow);

		Assert::IsTrue(hbrush_a == hbrush_b);
	}

	TEST_METHOD(WhenRequestingAYellowPen_ReturnAYellowPen) {
		GdiObjCache cache;

		HPEN hpen = cache.GetPen(Color::Yellow);

		auto actual_color = GetPenColor(hpen);
		Assert::AreEqual<COLORREF>(actual_color, Color::Yellow);
	}

	TEST_METHOD(WhenRequestingAYellowPenTwice_SamePenIsReturned) {
		GdiObjCache cache;

		HPEN hpen_a = cache.GetPen(Color::Yellow);
		HPEN hpen_b = cache.GetPen(Color::Yellow);

		Assert::IsTrue(hpen_a == hpen_b);
	}

private:
	COLORREF GetBrushColor(HBRUSH hbrush)
	{
		LOGBRUSH logbrush;

		if(!GetObject(hbrush, sizeof(logbrush), &logbrush))
			Assert::Fail();

		return logbrush.lbColor;
	}

	COLORREF GetPenColor(HPEN hpen)
	{
		LOGPEN logpen;

		if(!GetObject(hpen, sizeof(logpen), &logpen))
			Assert::Fail();

		return logpen.lopnColor;
	}
};
