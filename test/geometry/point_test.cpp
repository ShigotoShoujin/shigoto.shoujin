import Shoujin.Geometry;
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::geometry;

namespace shoujin::test::geometry {

TEST_CLASS(PointTest) {
	using Vector2d = Vector2d<int>;
	using Point = Point<int>;

public:
	TEST_METHOD(ConstructorVector2d_PointInitialized) {
		Vector2d vec{2, 3};
		Point pt{vec};
		Assert::AreEqual(2, pt.x);
		Assert::AreEqual(3, pt.y);
	}

	TEST_METHOD(ConstructorWithTwoTParam_PointInitialized) {
		Point pt{1, 2};
		Assert::AreEqual(1, pt.x);
		Assert::AreEqual(2, pt.y);
	}

	TEST_METHOD(DefaultConstructor_PointIsAtZero) {
		Point pt;
		Assert::AreEqual(0, pt.x);
		Assert::AreEqual(0, pt.y);
	}
};

}
