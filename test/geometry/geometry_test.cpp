import Shoujin.Geometry;
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::geometry;

namespace shoujin::test::geometry {

TEST_CLASS(GeometryTest) {
	using Point = Point<int>;
	using Rectangle = Rectangle<int>;

public:
	TEST_METHOD(ClampPointToRect_WhenPointInsideRect_NoChange) {
		Rectangle rect{10, 10, 20, 20};
		Point pt{15, 15};

		auto&& actual = ClampPointToRect(pt, rect);

		Assert::AreEqual(pt.x, actual.x);
		Assert::AreEqual(pt.y, actual.y);
	}

	TEST_METHOD(ClampPointToRect_WhenPointOnBottomLine_NoChange) {
		Rectangle rect{10, 10, 20, 20};
		Point pt{15, 19};

		auto&& actual = ClampPointToRect(pt, rect);

		Assert::AreEqual(pt.x, actual.x);
		Assert::AreEqual(pt.y, actual.y);
	}

	TEST_METHOD(ClampPointToRect_WhenPointOnLeftLine_NoChange) {
		Rectangle rect{10, 10, 20, 20};
		Point pt{10, 15};

		auto&& actual = ClampPointToRect(pt, rect);

		Assert::AreEqual(pt.x, actual.x);
		Assert::AreEqual(pt.y, actual.y);
	}

	TEST_METHOD(ClampPointToRect_WhenPointOnRightLine_NoChange) {
		Rectangle rect{10, 10, 20, 20};
		Point pt{19, 15};

		auto&& actual = ClampPointToRect(pt, rect);

		Assert::AreEqual(pt.x, actual.x);
		Assert::AreEqual(pt.y, actual.y);
	}

	TEST_METHOD(ClampPointToRect_WhenPointOnTopLine_NoChange) {
		Rectangle rect{10, 10, 20, 20};
		Point pt{15, 10};

		auto&& actual = ClampPointToRect(pt, rect);

		Assert::AreEqual(pt.x, actual.x);
		Assert::AreEqual(pt.y, actual.y);
	}

	TEST_METHOD(ClampPointToRect_WhenPointTooFarDown_PointMovedToBottomLine) {
		Rectangle rect{10, 10, 20, 20};
		Point pt{15, 25};

		auto&& actual = ClampPointToRect(pt, rect);

		Assert::AreEqual(pt.x, actual.x);
		Assert::AreEqual(19, actual.y);
	}

	TEST_METHOD(ClampPointToRect_WhenPointTooFarLeft_PointMovedToLeftLine) {
		Rectangle rect{10, 10, 20, 20};
		Point pt{5, 15};

		auto&& actual = ClampPointToRect(pt, rect);

		Assert::AreEqual(10, actual.x);
		Assert::AreEqual(pt.y, actual.y);
	}

	TEST_METHOD(ClampPointToRect_WhenPointTooFarRight_PointMovedToRightLine) {
		Rectangle rect{10, 10, 20, 20};
		Point pt{25, 15};

		auto&& actual = ClampPointToRect(pt, rect);

		Assert::AreEqual(19, actual.x);
		Assert::AreEqual(pt.y, actual.y);
	}

	TEST_METHOD(ClampPointToRect_WhenPointTooFarUp_PointMovedToTopLine) {
		Rectangle rect{10, 10, 20, 20};
		Point pt{15, 5};

		auto&& actual = ClampPointToRect(pt, rect);

		Assert::AreEqual(pt.x, actual.x);
		Assert::AreEqual(10, actual.y);
	}
};

}
