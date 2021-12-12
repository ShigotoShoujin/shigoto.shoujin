#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../src/grid/grid2.hpp"
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

using namespace shoujin;

TEST_CLASS(Grid_Row2Test) {
	TEST_METHOD(Alias_ValueType_IsType) {
		Assert::IsTrue(std::is_same_v<Row2::value_type, int>);
	}

	TEST_METHOD(Alias_Pointer_IsTypePointer) {
		Assert::IsTrue(std::is_same_v<Row2::pointer, int*>);
	}

	TEST_METHOD(Alias_ConstPointer_IsTypeConstPointer) {
		Assert::IsTrue(std::is_same_v<Row2::const_pointer, int const*>);
	}

	TEST_METHOD(Alias_Iterator_IsTypePointer) {
		Assert::IsTrue(std::is_same_v<Row2::iterator, Row2::pointer>);
	}

	TEST_METHOD(Alias_ConstIterator_IsTypeConstPointer) {
		Assert::IsTrue(std::is_same_v<Row2::const_iterator, Row2::const_pointer>);
	}

	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Row2>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Row2>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Row2>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Row2>);
	}

	TEST_METHOD(OperatorComparison_WhenBeginEndMismatch_ReturnFalse) {
		int data1[]{1, 2};
		int data2[]{1, 2};
		Row2 row1{data1, data1 + 2};
		Row2 row2{data2, data2 + 2};

		auto actual = row1 == row2;

		Assert::IsFalse(actual);
	}

	TEST_METHOD(OperatorComparison_WhenBeginEndMatch_ReturnTrue) {
		int data[]{1, 2};
		Row2 row1{data, data + 2};
		Row2 row2{data, data + 2};

		auto actual = row1 == row2;

		Assert::IsTrue(actual);
	}

	TEST_METHOD(RangeFor_EnumerateEachValue) {
		int data[]{1, 2, 3};
		Row2 row{data, data + 3};
		auto i = 0;

		for(auto&& it : row)
			Assert::AreEqual(++i, it);
	}

	TEST_METHOD(RangeForConst_EnumerateEachValue) {
		int data[]{1, 2, 3};
		Row2 row{data, data + 3};
		auto& const_row = std::as_const(row);
		auto i = 0;

		for(auto&& it : const_row)
			Assert::AreEqual(++i, it);
	}
};

TEST_CLASS(Grid_ConstRowIterator2Test) {
	TEST_METHOD(Alias_ValueType_IsType) {
		Assert::IsTrue(std::is_same_v<ConstRowIterator2::value_type, Row2>);
	}

	TEST_METHOD(Alias_Reference_IsTypeReference) {
		Assert::IsTrue(std::is_same_v<ConstRowIterator2::reference, ConstRowIterator2::value_type const&>);
	}

	TEST_METHOD(Alias_Pointer_IsTypePointer) {
		Assert::IsTrue(std::is_same_v<ConstRowIterator2::pointer, ConstRowIterator2::value_type const*>);
	}

	TEST_METHOD(Alias_DifferenceType_IsPtrDiffT) {
		Assert::IsTrue(std::is_same_v<ConstRowIterator2::difference_type, ptrdiff_t>);
	}

	TEST_METHOD(Alias_IteratorCategory_IsRandomAccessIterator) {
		Assert::IsTrue(std::is_same_v<ConstRowIterator2::iterator_category, std::random_access_iterator_tag>);
	}

	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<ConstRowIterator2>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<ConstRowIterator2>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<ConstRowIterator2>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<ConstRowIterator2>);
	}

	TEST_METHOD(IsDestructible) {
		Assert::IsTrue(std::is_destructible_v<ConstRowIterator2>);
	}

	TEST_METHOD(Size_ReturnRowLength) {
		ConstRowIterator2 c_row_it{nullptr, 3};
		Assert::AreEqual<size_t>(3, c_row_it.size());
	}

	TEST_METHOD(IsSwappable) {
		ConstRowIterator2 a{nullptr, 1};
		ConstRowIterator2 b{nullptr, 2};

		using std::swap;
		swap(a, b);

		Assert::AreEqual<ConstRowIterator2::difference_type>(2, a.size());
		Assert::AreEqual<ConstRowIterator2::difference_type>(1, b.size());
	}

	TEST_METHOD(OperatorIndirection_ReturnCurrentRow) {
		int data[]{1, 2, 3};
		ConstRowIterator2 c_row_it{data, 3};

		auto&& actual = *c_row_it;

		Assert::AreEqual(*(data + 0), *actual.begin());
		Assert::AreEqual(*(data + 3), *actual.end());
	}

	TEST_METHOD(OperatorDereference_ReturnCurrentRow) {
		int data[]{1, 2, 3};
		ConstRowIterator2 c_row_it{data, 3};

		auto&& actual = c_row_it.operator->();

		Assert::AreEqual(*(data + 0), *actual->begin());
		Assert::AreEqual(*(data + 3), *actual->end());
	}

	TEST_METHOD(OperatorIncrementPrefix_MoveToNextRowAndReturnIt) {
		int data[]{1, 2, 3, 4};
		ConstRowIterator2 c_row_it{data, 2};

		auto&& actual = ++c_row_it;

		Assert::AreEqual(*(data + 2), *actual->begin());
	}

	TEST_METHOD(OperatorIncrementPostfix_MoveToNextRowAndReturnPreviousRow) {
		int data[]{1, 2, 3, 4};
		ConstRowIterator2 c_row_it{data, 2};

		auto&& actual = c_row_it++;

		Assert::AreEqual(*data, *(*actual).begin());
		Assert::AreEqual(*(data + 2), *c_row_it->begin());
	}

	TEST_METHOD(OperatorComparison_WhenReferencingOtherRow_ReturnFalse) {
		int data1[]{1, 2};
		int data2[]{1, 2};
		ConstRowIterator2 c_row_it1{data1, 2};
		ConstRowIterator2 c_row_it2{data2, 2};

		auto actual = c_row_it1 == c_row_it2;

		Assert::IsFalse(actual);
	}

	TEST_METHOD(OperatorComparison_WhenReferencingSameRowMatch_ReturnTrue) {
		int data[]{1, 2};
		ConstRowIterator2 c_row_it1{data, 2};
		ConstRowIterator2 c_row_it2{data, 2};

		auto actual = c_row_it1 == c_row_it2;

		Assert::IsTrue(actual);
	}
};

TEST_CLASS(Grid_RowIterator2Test) {
	TEST_METHOD(Alias_ValueType_IsType) {
		Assert::IsTrue(std::is_same_v<RowIterator2::value_type, Row2>);
	}

	TEST_METHOD(Alias_Reference_IsTypeReference) {
		Assert::IsTrue(std::is_same_v<RowIterator2::reference, RowIterator2::value_type&>);
	}

	TEST_METHOD(Alias_Pointer_IsTypePointer) {
		Assert::IsTrue(std::is_same_v<RowIterator2::pointer, RowIterator2::value_type*>);
	}

	TEST_METHOD(Alias_DifferenceType_IsPtrDiffT) {
		Assert::IsTrue(std::is_same_v<RowIterator2::difference_type, ptrdiff_t>);
	}

	TEST_METHOD(Alias_IteratorCategory_IsRandomAccessIterator) {
		Assert::IsTrue(std::is_same_v<RowIterator2::iterator_category, std::random_access_iterator_tag>);
	}

	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<RowIterator2>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<RowIterator2>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<RowIterator2>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<RowIterator2>);
	}

	TEST_METHOD(IsDestructible) {
		Assert::IsTrue(std::is_destructible_v<RowIterator2>);
	}

	TEST_METHOD(Size_ReturnRowLength) {
		RowIterator2 row_it{nullptr, 3};
		Assert::AreEqual<size_t>(3, row_it.size());
	}

	TEST_METHOD(IsSwappable) {
		RowIterator2 a{nullptr, 1};
		RowIterator2 b{nullptr, 2};

		using std::swap;
		swap(a, b);

		Assert::AreEqual<RowIterator2::difference_type>(2, a.size());
		Assert::AreEqual<RowIterator2::difference_type>(1, b.size());
	}

	TEST_METHOD(OperatorIndirection_ReturnCurrentRow) {
		int data[]{1, 2, 3};
		RowIterator2 row_it{data, 3};

		auto&& actual = *row_it;

		Assert::AreEqual(*(data + 0), *actual.begin());
		Assert::AreEqual(*(data + 3), *actual.end());
	}

	TEST_METHOD(OperatorDereference_ReturnCurrentRow) {
		int data[]{1, 2, 3};
		RowIterator2 row_it{data, 3};

		auto&& actual = row_it.operator->();

		Assert::AreEqual(*(data + 0), *actual->begin());
		Assert::AreEqual(*(data + 3), *actual->end());
	}

	TEST_METHOD(OperatorIncrementPrefix_MoveToNextRowAndReturnIt) {
		int data[]{1, 2, 3, 4};
		RowIterator2 row_it{data, 2};

		auto&& actual = ++row_it;

		Assert::AreEqual(*(data + 2), *actual->begin());
	}

	TEST_METHOD(OperatorIncrementPostfix_MoveToNextRowAndReturnPreviousRow) {
		int data[]{1, 2, 3, 4};
		RowIterator2 row_it{data, 2};

		auto&& actual = row_it++;

		Assert::AreEqual(*data, *(*actual).begin());
		Assert::AreEqual(*(data + 2), *row_it->begin());
	}

	TEST_METHOD(OperatorComparison_WhenReferencingOtherRow_ReturnFalse) {
		int data1[]{1, 2};
		int data2[]{1, 2};
		RowIterator2 row_it1{data1, 2};
		RowIterator2 row_it2{data2, 2};

		auto actual = row_it1 == row_it2;

		Assert::IsFalse(actual);
	}

	TEST_METHOD(OperatorComparison_WhenReferencingSameRowMatch_ReturnTrue) {
		int data[]{1, 2};
		RowIterator2 row_it1{data, 2};
		RowIterator2 row_it2{data, 2};

		auto actual = row_it1 == row_it2;

		Assert::IsTrue(actual);
	}
};

TEST_CLASS(Grid_Grid2Test) {
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
	TEST_METHOD(Alias_ValueType_IsType) {
		Assert::IsTrue(std::is_same_v<Grid2::value_type, int>);
	}

	TEST_METHOD(Alias_Reference_IsTypeReference) {
		Assert::IsTrue(std::is_same_v<Grid2::reference, int&>);
	}

	TEST_METHOD(Alias_ConstReference_IsConstTypeRef) {
		Assert::IsTrue(std::is_same_v<Grid2::const_reference, int const&>);
	}

	TEST_METHOD(Alias_Iterator_IsVectorTypeIterator) {
		Assert::IsTrue(std::is_same_v<Grid2::iterator, std::vector<int>::iterator>);
	}

	TEST_METHOD(Alias_ConstIterator_IsVectorTypeConstIterator) {
		Assert::IsTrue(std::is_same_v<Grid2::const_iterator, std::vector<int>::const_iterator>);
	}

	TEST_METHOD(Alias_DifferenceType_IsVectorTypeDifferenceType) {
		Assert::IsTrue(std::is_same_v<Grid2::difference_type, std::vector<int>::difference_type>);
	}

	TEST_METHOD(Alias_SizeType_IsVectorTypeSizeType) {
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

	TEST_METHOD(MaxSize_GreaterThanOne) {
		Grid2 grid(5, 3);

		auto actual = grid.max_size();

		Assert::IsTrue(actual > 0);
	}

	TEST_METHOD(Data_ReturnSameAsBegin) {
		Grid2 grid(5, 3);
		Assert::IsTrue(*grid.data() == *grid.begin());
	}

	TEST_METHOD(DataConst_ReturnSameAsBegin) {
		Grid2 grid(5, 3);
		auto& const_grid = std::as_const(grid);
		Assert::IsTrue(*const_grid.data() == *const_grid.begin());
	}

	TEST_METHOD(Empty_GridIsEmpty_ReturnTrue) {
		Grid2 grid;
		Assert::IsTrue(grid.empty());
	}

	TEST_METHOD(Empty_GridIsNotEmpty_ReturnFalse) {
		Grid2 grid(3, 3);
		Assert::IsFalse(grid.empty());
	}

	TEST_METHOD(OperatorSubscript_ReturnValue) {
		Grid2 grid(2, 2);
		*(grid.data() + 2) = 53;

		auto actual = grid[2];

		Assert::AreEqual(53, actual);
	}

	TEST_METHOD(OperatorSubscriptConst_ReturnValue) {
		Grid2 grid(2, 2);
		*(grid.data() + 2) = 53;
		auto& const_grid = std::as_const(grid);

		auto actual = const_grid[2];

		Assert::AreEqual(53, actual);
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

	TEST_METHOD(OperatorComparison_WhenSizeMismatch_ReturnFalse) {
		auto a = CreateFilledGrid(5, 3);
		auto b = CreateFilledGrid(3, 5);

		Assert::IsFalse(a == b);
		Assert::IsTrue(a != b);
	}

	TEST_METHOD(OperatorComparison_WhenDataMismatch_ReturnFalse) {
		auto a = CreateFilledGrid(2, 3);
		auto b = Grid2(2, 3);

		Assert::IsFalse(a == b);
		Assert::IsTrue(a != b);
	}

	TEST_METHOD(OperatorComparison_WhenDataMatch_ReturnTrue) {
		auto a = CreateFilledGrid(2, 3);
		auto b = CreateFilledGrid(2, 3);

		Assert::IsTrue(a == b);
		Assert::IsFalse(a != b);
	}

	TEST_METHOD(Swap_MemberFunction_Swapped) {
		Grid2 a(2, 3);
		Grid2 b(3, 2);

		a.swap(b);

		Assert::AreEqual<size_t>(3, a.width());
		Assert::AreEqual<size_t>(2, b.width());
	}

	TEST_METHOD(Swap_GlobalFunction_Swapped) {
		Grid2 a(2, 3);
		Grid2 b(3, 2);

		swap(a, b);

		Assert::AreEqual<size_t>(3, a.width());
		Assert::AreEqual<size_t>(2, b.width());
	}

	TEST_METHOD(Initializer_Width_ReturnWidth) {
		Grid2 grid{1, 2, {}};
		Assert::AreEqual<size_t>(1, grid.width());
	}

	TEST_METHOD(Initializer_Height_ReturnHeight) {
		Grid2 grid{1, 2, {}};
		Assert::AreEqual<size_t>(2, grid.height());
	}

	TEST_METHOD(Initializer_Empty_AllItemsHaveDefaultValue) {
		Grid2 grid{1, 2, {}};
		Assert::IsTrue(std::all_of(grid.begin(), grid.end(), [](int i) { return i == 0; }));
	}

	TEST_METHOD(Initializer_AllItemsInitialized) {
		auto values = {4, 3, 2, 6, 4, 1};

		Grid2 grid{3, 2, values};

		Assert::IsTrue(std::equal(grid.begin(), grid.end(), values.begin()));
	}

	TEST_METHOD(Initializer_TooManyItems_ExtraItemsIgnored) {
		Grid2 grid{1, 2, {4, 5, 6, 7}};

		auto expected = {4, 5, 6};
		Assert::IsTrue(std::equal(grid.begin(), grid.end(), expected.begin()));
	}

	TEST_METHOD(Initializer_NotEnoughItems_RemainingItemsHaveDefaultValue) {
		Grid2 grid{1, 2, {1, 2}};

		auto expected = {1, 2, 0, 0};
		Assert::IsTrue(std::equal(grid.begin(), grid.end(), expected.begin()));
	}
};
