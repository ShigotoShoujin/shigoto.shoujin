#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../src/grid/grid2.hpp"
#include <iterator>
#include <utility>
#include <vector>

using namespace shoujin;

TEST_CLASS(Grid2Test) {
	Grid2 CreateFilledGrid(size_t width, size_t height)
	{
		Grid2 grid(width, height);

		auto&& it = grid.data();
		auto&& end = it + grid.size();
		auto i = 0;
		while(it != end)
			*it++ = i++;

		return grid;
	}

public:
	TEST_METHOD(HasAlias_ValueType_IsType) {
		Assert::IsTrue(std::is_same_v<Grid2::value_type, int>);
	}

	TEST_METHOD(HasAlias_Reference_IsTypeRef) {
		Assert::IsTrue(std::is_same_v<Grid2::reference, int&>);
	}

	TEST_METHOD(HasAlias_ConstReference_IsConstTypeRef) {
		Assert::IsTrue(std::is_same_v<Grid2::const_reference, int const&>);
	}

	TEST_METHOD(HasAlias_Iterator_IsVectorTypeIterator) {
		Assert::IsTrue(std::is_same_v<Grid2::iterator, std::vector<int>::iterator>);
	}

	TEST_METHOD(HasAlias_ConstIterator_IsVectorTypeConstIterator) {
		Assert::IsTrue(std::is_same_v<Grid2::const_iterator, std::vector<int>::const_iterator>);
	}

	TEST_METHOD(HasAlias_DifferenceType_IsVectorTypeDifferenceType) {
		Assert::IsTrue(std::is_same_v<Grid2::difference_type, std::vector<int>::difference_type>);
	}

	TEST_METHOD(HasAlias_SizeType_IsVectorTypeSizeType) {
		Assert::IsTrue(std::is_same_v<Grid2::size_type, std::vector<int>::size_type>);
	}

	TEST_METHOD(IsDefaultConstructible) {
		Assert::IsTrue(std::is_default_constructible_v<Grid2>);
	}

	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Grid2>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Grid2>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Grid2>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Grid2>);
	}

	TEST_METHOD(IsDestructible) {
		Assert::IsTrue(std::is_destructible_v<Grid2>);
	}

	TEST_METHOD(Width_ReturnWidth) {
		Grid2 grid(5, 3);
		Assert::AreEqual<size_t>(5, grid.width());
	}

	TEST_METHOD(Height_ReturnHeight) {
		Grid2 grid(5, 3);
		Assert::AreEqual<size_t>(3, grid.height());
	}

	TEST_METHOD(Size_ReturnWidthTimesHeight) {
		Grid2 grid(5, 3);
		Assert::AreEqual<size_t>(15, grid.size());
	}

	TEST_METHOD(Data_ReturnSameAsBegin) {
		Grid2 grid(5, 3);
		Assert::IsTrue(*grid.data() == *grid.begin());
	}

	TEST_METHOD(BeginEnd_DifferenceIsSameAsSize) {
		Grid2 grid(5, 3);
		auto diff = grid.end() - grid.begin();
		Assert::IsTrue(diff == grid.size());
	}

	TEST_METHOD(RangeFor_EnumerateEachValue) {
		auto grid = CreateFilledGrid(5, 3);
		auto i = 0;

		for(auto&& it : grid)
			Assert::AreEqual(i++, it);
	}

	TEST_METHOD(RangeForConst_EnumerateEachValue) {
		auto grid = CreateFilledGrid(5, 3);
		auto i = 0;

		for(auto&& it : std::as_const(grid))
			Assert::AreEqual(i++, it);
	}

	TEST_METHOD(ComparisonOperator_WhenSizeMismatch_ReturnFalse) {
		auto a = CreateFilledGrid(5, 3);
		auto b = CreateFilledGrid(3, 5);

		Assert::IsFalse(a == b);
		Assert::IsTrue(a != b);
	}

	TEST_METHOD(ComparisonOperator_WhenDataMismatch_ReturnFalse) {
		auto a = CreateFilledGrid(2, 3);
		auto b = Grid2(2, 3);

		Assert::IsFalse(a == b);
		Assert::IsTrue(a != b);
	}

	TEST_METHOD(ComparisonOperator_WhenDataMatch_ReturnTrue) {
		auto a = CreateFilledGrid(2, 3);
		auto b = CreateFilledGrid(2, 3);

		Assert::IsTrue(a == b);
		Assert::IsFalse(a != b);
	}

	//TEST_METHOD(ConstLinearIterate_OK) {
	//	//Arrange
	//	const Grid2 grid(2, 3);
	//	std::vector<int> result;

	//	//Act
	//	for(auto&& it : grid)
	//		result.push_back(it);

	//	//Assert
	//	Assert::AreEqual(grid.size(), result.size());
	//	for(int i : {1, 2, 3, 4, 5, 6})
	//		Assert::AreEqual(i, result[i]);
	//}

	//TEST_METHOD(ConstIterateRangeFor_OK) {
	//}
};
