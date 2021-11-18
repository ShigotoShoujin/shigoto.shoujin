#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/gui.hpp>

using namespace shoujin::gui;

TEST_CLASS(RectTest) {
public:
	TEST_METHOD(PositionAndSize_WidthAndHeightOk) {
		int x1 = 10, y1 = 11, x2 = 15, y2 = 16;
		Rect rect{x1, y1, x2, y2};

		Assert::AreEqual(x2 - x1, rect.width());
		Assert::AreEqual(y2 - y1, rect.height());
	}

	TEST_METHOD(OperatorMul_NewSizeOk) {
		Rect rect {10, 11, 16, 20};

		auto actual = rect * 2;

		Assert::AreEqual(22, actual.x2);
		Assert::AreEqual(29, actual.y2);
	}

	TEST_METHOD(OperatorDiv_NewSizeOk) {
		Rect rect{10, 11, 16, 20};

		auto actual = rect / 2;

		Assert::AreEqual(13, actual.x2);
		Assert::AreEqual(15, actual.y2);
	}

	TEST_METHOD(CtorPointAndSize_RectOk) {
		int x = 10, y = 11;
		int w = 15, h = 16;
		Rect rect{{x, y}, {w, h}};
		
		Assert::AreEqual(x, rect.x1);
		Assert::AreEqual(y, rect.y1);
		Assert::AreEqual(x + w, rect.x2);
		Assert::AreEqual(y + h, rect.y2);
	}

	TEST_METHOD(CtorRectOk) {
		RECT rect{10, 11, 16, 20};

		Assert::AreEqual(10, static_cast<int>(rect.left));
		Assert::AreEqual(11, static_cast<int>(rect.top));
		Assert::AreEqual(16, static_cast<int>(rect.right));
		Assert::AreEqual(20, static_cast<int>(rect.bottom));
	}
};
