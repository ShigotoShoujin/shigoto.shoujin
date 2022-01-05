#include "../../../src/gui/bitmap/brush_cache.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::gui;
using namespace shoujin::gui::bitmap;

TEST_CLASS(BrushCacheTest) {
public:
	TEST_METHOD(GivenAnEmptyBrushCache_WhenRequestingAYellowBrush_ReturnAYellowBrush) {
		BrushCache cache;

		HBRUSH hbrush = cache.GetBrush(Color::Yellow);

		auto brush_info = GetBrushObject(hbrush);
		Assert::AreEqual<COLORREF>(brush_info.lbColor, Color::Yellow);
	}

	TEST_METHOD(GivenAnEmptyBrushCache_WhenRequestingABrush_BrushIsCached) {
		BrushCache cache;

		HBRUSH brush = cache.GetBrush(Color::Yellow);

		Assert::IsTrue(cache.GetCache().contains(Color::Yellow));
	}

	TEST_METHOD(GivenAnBrushCacheWithABrush_WhenRequestingADifferentBrush_BothBrushesAreCached) {
		BrushCache cache;
		cache.GetBrush(Color::Yellow);

		HBRUSH brush = cache.GetBrush(Color::Red);

		Assert::IsTrue(cache.GetCache().contains(Color::Red));
		Assert::IsTrue(cache.GetCache().contains(Color::Yellow));
	}

private:
	LOGBRUSH GetBrushObject(HBRUSH hbrush)
	{
		LOGBRUSH logbrush;

		if(!GetObject(hbrush, sizeof(logbrush), &logbrush))
			Assert::Fail();

		return logbrush;
	}
};
