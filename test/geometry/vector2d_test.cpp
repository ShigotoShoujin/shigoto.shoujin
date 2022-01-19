import Shoujin.Geometry;
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::geometry;

namespace shoujin::test::geometry {

TEST_CLASS(Vector2dTest) {
	using Vector2d = Vector2d<int>;

public:
	TEST_METHOD(ConstructorWithTwoTParam_VectorInitialized) {
		Vector2d vec{4, 5};
		Assert::AreEqual(4, vec.x);
		Assert::AreEqual(5, vec.y);
	}

	TEST_METHOD(DefaultConstructor_VectorIsAtZero) {
		Vector2d vec;
		Assert::AreEqual(0, vec.x);
		Assert::AreEqual(0, vec.y);
	}

	TEST_METHOD(TwoVectorWithDifferentValues_InequalityOperator_ReturnsTrue) {
		Vector2d vec1{3, 2};
		Vector2d vec2{2, 3};
		Assert::IsTrue(vec1 != vec2);
	}

	TEST_METHOD(TwoVectorWithDiffValues_EqualityOperator_ReturnsFalse) {
		Vector2d vec1{3, 2};
		Vector2d vec2{2, 3};
		Assert::IsFalse(vec1 == vec2);
	}

	TEST_METHOD(TwoVectorWithSameValues_EqualityOperator_ReturnsTrue) {
		Vector2d vec1{3, 3};
		Vector2d vec2{3, 3};
		Assert::IsTrue(vec1 == vec2);
	}

	TEST_METHOD(TwoVectorWithSameValues_InequalityOperator_ReturnsFalse) {
		Vector2d vec1{2, 2};
		Vector2d vec2{2, 2};
		Assert::IsFalse(vec1 != vec2);
	}

	TEST_METHOD(VectorAtZero_OperatorBoolReturnsFalse) {
		Vector2d vec;
		Assert::IsFalse(!!vec);
	}

	TEST_METHOD(VectorDivEqualValue_ReturnVectorRef) {
		Vector2d vec1{15, 25};

		auto&& vec2 = vec1 /= 5;

		Assert::IsTrue(&vec2 == &vec1);
		Assert::AreEqual(3, vec2.x);
		Assert::AreEqual(5, vec2.y);
	}

	TEST_METHOD(VectorDivValue_ReturnNewVector) {
		Vector2d vec1{15, 25};

		auto&& vec2 = vec1 / 5;

		Assert::AreEqual(3, vec2.x);
		Assert::AreEqual(5, vec2.y);
	}

	TEST_METHOD(VectorMinusEqualValue_ReturnVectorRef) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 -= 5;

		Assert::IsTrue(&vec2 == &vec1);
		Assert::AreEqual(-4, vec2.x);
		Assert::AreEqual(-2, vec2.y);
	}

	TEST_METHOD(VectorMinusEqualVector_ReturnVectorRef) {
		Vector2d vec1{1, 3};
		Vector2d vec2{3, 2};

		auto&& diff = vec1 -= vec2;

		Assert::IsTrue(&diff == &vec1);
		Assert::AreEqual(-2, diff.x);
		Assert::AreEqual(1, diff.y);
	}

	TEST_METHOD(VectorMinusValue_ReturnNewVector) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 - 5;

		Assert::AreEqual(-4, vec2.x);
		Assert::AreEqual(-2, vec2.y);
	}

	TEST_METHOD(VectorMinusVector_ReturnNewVector) {
		Vector2d vec1{1, 3};
		Vector2d vec2{3, 2};

		auto&& diff = vec1 - vec2;

		Assert::AreEqual(-2, diff.x);
		Assert::AreEqual(1, diff.y);
	}

	TEST_METHOD(VectorMulEqualValue_ReturnVectorRef) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 *= 5;

		Assert::IsTrue(&vec2 == &vec1);
		Assert::AreEqual(5, vec2.x);
		Assert::AreEqual(15, vec2.y);
	}

	TEST_METHOD(VectorMulValue_ReturnNewVector) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 * 5;

		Assert::AreEqual(5, vec2.x);
		Assert::AreEqual(15, vec2.y);
	}

	TEST_METHOD(VectorNotZero_OperatorBoolReturnsTrue) {
		Vector2d vec{3, 2};
		Assert::IsTrue(!!vec);
	}

	TEST_METHOD(VectorPlusEqualValue_Return­­VectorRef) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 += 5;

		Assert::IsTrue(&vec2 == &vec1);
		Assert::AreEqual(6, vec2.x);
		Assert::AreEqual(8, vec2.y);
	}

	TEST_METHOD(VectorPlusEqualVector_ReturnVectorRef) {
		Vector2d vec1{1, 3};
		Vector2d vec2{3, 2};

		auto&& sum = vec1 += vec2;

		Assert::IsTrue(&sum == &vec1);
		Assert::AreEqual(4, sum.x);
		Assert::AreEqual(5, sum.y);
	}

	TEST_METHOD(VectorPlusValue_ReturnNewVector) {
		Vector2d vec1{1, 3};

		auto&& vec2 = vec1 + 5;

		Assert::AreEqual(6, vec2.x);
		Assert::AreEqual(8, vec2.y);
	}

	TEST_METHOD(VectorPlusVector_ReturnNewVector) {
		Vector2d vec1{1, 3};
		Vector2d vec2{3, 2};

		auto&& sum = vec1 + vec2;

		Assert::AreEqual(4, sum.x);
		Assert::AreEqual(5, sum.y);
	}
};

}
