import Shoujin.Geometry;
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::geometry;

namespace shoujin::test::geometry {

TEST_CLASS(RectangleTest) {
	using Rectangle = Rectangle<int>;

public:
	TEST_METHOD(ConstructorPointAndSize_RectangleInitialized) {
		Rectangle rect{{5, 6}, {10, 10}};
		Assert::AreEqual(5, rect.x1);
		Assert::AreEqual(6, rect.y1);
		Assert::AreEqual(15, rect.x2);
		Assert::AreEqual(16, rect.y2);
	}

	TEST_METHOD(ConstructorWithFourTParam_RectangleInitialized) {
		Rectangle rect{10, 11, 20, 21};
		Assert::AreEqual(10, rect.x1);
		Assert::AreEqual(11, rect.y1);
		Assert::AreEqual(20, rect.x2);
		Assert::AreEqual(21, rect.y2);
	}

	TEST_METHOD(DefaultConstructor_RectangleIsAtZero) {
		Rectangle rect;
		Assert::AreEqual(0, rect.x1);
		Assert::AreEqual(0, rect.y1);
		Assert::AreEqual(0, rect.x2);
		Assert::AreEqual(0, rect.y2);
	}

	TEST_METHOD(RectangleToSize_Converted) {
		Rectangle rect{10, 11, 22, 25};

		auto&& result = RectangleToSize(rect);

		Assert::AreEqual(12, result.x);
		Assert::AreEqual(14, result.y);
	}

	TEST_METHOD(RectNotZero_OperatorBoolReturnsTrue) {
		Rectangle rect{10, 11, 22, 25};
		Assert::IsTrue(!!rect);
	}

	TEST_METHOD(RectZero_OperatorBoolReturnsFalse) {
		Rectangle rect;
		Assert::IsFalse(!!rect);
	}

	TEST_METHOD(TwoRectangleWithDifferentValues_InequalityOperator_ReturnsTrue) {
		Rectangle rect1{10, 11, 22, 25};
		Rectangle rect2{11, 31, 12, 45};
		Assert::IsTrue(rect1 != rect2);
	}

	TEST_METHOD(TwoRectangleWithDiffValues_EqualityOperator_ReturnsFalse) {
		Rectangle rect1{10, 11, 22, 25};
		Rectangle rect2{11, 31, 12, 45};
		Assert::IsFalse(rect1 == rect2);
	}

	TEST_METHOD(TwoRectangleWithSameValues_EqualityOperator_ReturnsTrue) {
		Rectangle rect1{10, 11, 22, 25};
		Rectangle rect2{10, 11, 22, 25};
		Assert::IsTrue(rect1 == rect2);
	}

	TEST_METHOD(TwoRectangleWithSameValues_InequalityOperator_ReturnsFalse) {
		Rectangle rect1{10, 11, 22, 25};
		Rectangle rect2{10, 11, 22, 25};
		Assert::IsFalse(rect1 != rect2);
	}

	TEST_METHOD(WidthHeight_ReturnSize) {
		Rectangle rect{10, 11, 22, 25};
		Assert::AreEqual(12, rect.width());
		Assert::AreEqual(14, rect.height());
	}
};

}
