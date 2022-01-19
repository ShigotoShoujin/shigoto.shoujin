import Shoujin.Geometry;
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::geometry;

namespace shoujin::test::geometry {

TEST_CLASS(Vector2dTest) {
	using Vector2d = Vector2d<int>;

public:
	TEST_METHOD(constructorWithTwoTParam_VectorInitialized) {
		Vector2d vec{4, 5};
		Assert::AreEqual(4, vec.x);
		Assert::AreEqual(5, vec.y);
	}

	TEST_METHOD(defaultConstructor_VectorIsAtZero) {
		Vector2d vec;
		Assert::AreEqual(0, vec.x);
		Assert::AreEqual(0, vec.y);
	}

	TEST_METHOD(twoVectorWithDifferentValues_InequalityOperator_ReturnsTrue) {
		Vector2d vec1{3, 2};
		Vector2d vec2{2, 3};
		Assert::IsTrue(vec1 != vec2);
	}

	TEST_METHOD(twoVectorWithDiffValues_EqualityOperator_ReturnsFalse) {
		Vector2d vec1{3, 2};
		Vector2d vec2{2, 3};
		Assert::IsFalse(vec1 == vec2);
	}

	TEST_METHOD(twoVectorWithSameValues_EqualityOperator_ReturnsTrue) {
		Vector2d vec1{3, 3};
		Vector2d vec2{3, 3};
		Assert::IsTrue(vec1 == vec2);
	}

	TEST_METHOD(twoVectorWithSameValues_InequalityOperator_ReturnsFalse) {
		Vector2d vec1{2, 2};
		Vector2d vec2{2, 2};
		Assert::IsFalse(vec1 != vec2);
	}

	TEST_METHOD(vectorAtZero_OperatorBoolReturnsFalse) {
		Vector2d vec;
		Assert::IsFalse(!!vec);
	}

	TEST_METHOD(vectorDivEqualValue_ReturnVectorRef) {
		Vector2d vec1{15, 25};

		auto&& vec2 = vec1 /= 5;

		Assert::IsTrue(&vec2 == &vec1);
		Assert::AreEqual(3, vec2.x);
		Assert::AreEqual(5, vec2.y);
	}

	TEST_METHOD(vectorDivValue_ReturnNewVector) {
		Vector2d vec1{15, 25};

		auto&& vec2 = vec1 / 5;

		Assert::AreEqual(3, vec2.x);
		Assert::AreEqual(5, vec2.y);
	}

	TEST_METHOD(vectorMinusEqualValue_ReturnVectorRef) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 -= 5;

		Assert::IsTrue(&vec2 == &vec1);
		Assert::AreEqual(-4, vec2.x);
		Assert::AreEqual(-2, vec2.y);
	}

	TEST_METHOD(vectorMinusEqualVector_ReturnVectorRef) {
		Vector2d vec1{1, 3};
		Vector2d vec2{3, 2};

		auto&& diff = vec1 -= vec2;

		Assert::IsTrue(&diff == &vec1);
		Assert::AreEqual(-2, diff.x);
		Assert::AreEqual(1, diff.y);
	}

	TEST_METHOD(vectorMinusValue_ReturnNewVector) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 - 5;

		Assert::AreEqual(-4, vec2.x);
		Assert::AreEqual(-2, vec2.y);
	}

	TEST_METHOD(vectorMinusVector_ReturnNewVector) {
		Vector2d vec1{1, 3};
		Vector2d vec2{3, 2};

		auto&& diff = vec1 - vec2;

		Assert::AreEqual(-2, diff.x);
		Assert::AreEqual(1, diff.y);
	}

	TEST_METHOD(vectorMulEqualValue_ReturnVectorRef) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 *= 5;

		Assert::IsTrue(&vec2 == &vec1);
		Assert::AreEqual(5, vec2.x);
		Assert::AreEqual(15, vec2.y);
	}

	TEST_METHOD(vectorMulValue_ReturnNewVector) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 * 5;

		Assert::AreEqual(5, vec2.x);
		Assert::AreEqual(15, vec2.y);
	}

	TEST_METHOD(vectorNotZero_OperatorBoolReturnsTrue) {
		Vector2d vec{3, 2};
		Assert::IsTrue(!!vec);
	}

	TEST_METHOD(vectorPlusEqualValue_Return­­VectorRef) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 += 5;

		Assert::IsTrue(&vec2 == &vec1);
		Assert::AreEqual(6, vec2.x);
		Assert::AreEqual(8, vec2.y);
	}

	TEST_METHOD(vectorPlusEqualVector_ReturnVectorRef) {
		Vector2d vec1{1, 3};
		Vector2d vec2{3, 2};

		auto&& sum = vec1 += vec2;

		Assert::IsTrue(&sum == &vec1);
		Assert::AreEqual(4, sum.x);
		Assert::AreEqual(5, sum.y);
	}

	TEST_METHOD(vectorPlusValue_ReturnNewVector) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 + 5;

		Assert::AreEqual(6, vec2.x);
		Assert::AreEqual(8, vec2.y);
	}

	TEST_METHOD(vectorPlusVector_ReturnNewVector) {
		Vector2d vec1{1, 3};
		Vector2d vec2{3, 2};

		auto&& sum = vec1 + vec2;

		Assert::AreEqual(4, sum.x);
		Assert::AreEqual(5, sum.y);
	}
};

}
