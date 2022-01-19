import Shoujin.Geometry;
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::geometry;

namespace shoujin::test::geometry {

TEST_CLASS(SizeTest) {
	using Vector2d = Vector2d<int>;
	using Size = Size<int>;

public:
	TEST_METHOD(constructorVector2d_SizeInitialized) {
		Vector2d vec{2, 3};
		Size sz{vec};
		Assert::AreEqual(2, sz.x);
		Assert::AreEqual(3, sz.y);
	}

	TEST_METHOD(constructorWithTwoTParam_SizeInitialized) {
		Size sz{1, 2};
		Assert::AreEqual(1, sz.x);
		Assert::AreEqual(2, sz.y);
	}

	TEST_METHOD(defaultConstructor_SizeIsAtZero) {
		Size sz;
		Assert::AreEqual(0, sz.x);
		Assert::AreEqual(0, sz.y);
	}
};

}
