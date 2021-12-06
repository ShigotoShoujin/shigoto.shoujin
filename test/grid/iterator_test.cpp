#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../src/grid/iterator.hpp"
#include <array>

using namespace shoujin;

TEST_CLASS(IteratorTest) {
	struct Pixel {
		uint8_t r{160}, g{80}, b{255};
	};

public:
	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Iterator<int>>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Iterator<int>>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Iterator<int>>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Iterator<int>>);
	}

	TEST_METHOD(DeferenceOperator_OK) {
		//Arrange
		auto arr = _array_of_five_int();
		Iterator<int> fsi{arr.data()};

		//Act
		int result = fsi.operator*();

		//Assert
		Assert::AreEqual(1, result);
	}

	TEST_METHOD(AccessOperator_OK) {
		//Arrange
		auto arr = _array_of_five_int();
		Iterator<int> fsi{arr.data()};

		//Act
		int* result = fsi.operator->();

		//Assert
		Assert::AreEqual(1, *result);
	}

	TEST_METHOD(IncrementOperator_OK) {
		//Arrange
		auto arr = _array_of_five_int();
		Iterator<int> fsi{arr.data()};

		//Act
		int result_a = *++fsi;
		int result_b = *fsi;

		//Assert
		Assert::AreEqual(2, result_a);
		Assert::AreEqual(2, result_b);
	}

	TEST_METHOD(PostIncrementOperator_OK) {
		//Arrange
		auto arr = _array_of_five_int();
		Iterator<int> fsi{arr.data()};

		//Act
		int result_a = *fsi++;
		int result_b = *fsi;

		//Assert
		Assert::AreEqual(1, result_a);
		Assert::AreEqual(2, result_b);
	}

	TEST_METHOD(EqualityOperator_OK) {
		//Arrange
		auto arr = _array_of_five_int();
		Iterator<int> fsi{arr.data()};

		//Assert
		Assert::IsTrue(fsi == fsi);
	}

	TEST_METHOD(InequalityOperator_OK) {
		//Arrange
		auto arr = _array_of_five_int();
		Iterator<int> fsi{arr.data()};

		//Act
		auto next = ++fsi;

		//Assert
		Assert::IsFalse(next != fsi);
	}

	TEST_METHOD(IterateStepOne_OK) {
		//Arrange
		auto arr = _array_of_five_int();
		Iterator<int> fsi{arr.data()};

		//Assert
		Assert::AreEqual(1, *fsi++);
		Assert::AreEqual(2, *fsi++);
		Assert::AreEqual(3, *fsi++);
		Assert::AreEqual(4, *fsi++);
		Assert::AreEqual(5, *fsi++);
	}

	TEST_METHOD(IterateStepTwo_OK) {
		//Arrange
		auto arr = _array_of_five_int();
		Iterator<int> fsi{arr.data(), 2};

		//Assert
		Assert::AreEqual(1, *fsi++);
		Assert::AreEqual(3, *fsi++);
		Assert::AreEqual(5, *fsi++);
	}

	TEST_METHOD(ObjectIterateStepTwo_OK) {
		//Arrange
		auto arr = std::array<Pixel, 10>();

		//Act
		auto begin = Iterator<Pixel>{arr.data(), 2};
		auto end = Iterator<Pixel>{arr.data() + arr.size(), 2};
		for(auto it = begin; it != end; ++it)
			it->r = 50;

		//Assert
		for(auto it = begin; it != end; ++it)
			Assert::AreEqual<uint8_t>(50, it->r);
	}

	TEST_METHOD(ConstIterate_OK) {
		Assert::Fail();
	}

private:
	std::array<int, 5> _array_of_five_int()
	{
		return {1, 2, 3, 4, 5};
	}
};

//
//#include <vector>
//void test()
//{
//	std::vector<int> v = {1, 2, 3, 4, 5};
//	auto&& begin = v.cbegin();
//	auto&& end = v.cend();
//
//	for(auto&& it = begin; it != end; ++it)
//		*it = 5;
//
//}
