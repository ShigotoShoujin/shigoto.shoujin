#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../src/grid.hpp"
#include <algorithm>
#include <array>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

using namespace shoujin;

namespace assist {

class GridVectorAsserter {
public:
	GridVectorAsserter(int width, int height) :
		_width{width},
		_height{height},
		_expected{height} {}

	void Push(int row, int value) { _expected[row].push_back(value); }

	template<int Size>
	void AssertSameAsArray(std::array<int, Size> const array)
	{
		_assert_all_rows_same_size();
		_assert_match_array(array);
	}

private:
	int _width, _height;
	std::vector<std::vector<int>> _expected;

	void _assert_all_rows_same_size()
	{
		auto size = _expected[0].size();
		for(auto& row : _expected)
			Assert::AreEqual(size, row.size());
	}

	template<int Size>
	void _assert_match_array(std::array<int, Size> const array)
	{
		for(int i = 0; i < _height; ++i)
			Assert::IsTrue(_compare_row(_expected[i].data(), array.data() + i * _width));
	}

	bool _compare_row(int const* actual, int const* expected)
	{
		bool result = true;
		for(int i = 0; i < _width; ++i, ++actual, ++expected)
			result &= *actual == *expected;
		return result;
	}
};

template<int Size>
std::array<int, Size> CreateIntArray()
{
	std::array<int, Size> array;
	for(int i = 0; i < Size; ++i)
		array[i] = i + 1;
	return array;
}

template<typename T>
Grid<T> CreateFilledGrid(size_t width, size_t height)
{
	Grid<T> grid(width, height);

	auto&& it = grid.data();
	auto&& end = it + grid.size();
	auto i = 0;
	while(it != end)
		*it++ = ++i;

	return grid;
}

}

using T = int;
using Row = Grid<T>::Row;
using ConstRowIterator = Grid<T>::ConstRowIterator;
using RowIterator = Grid<T>::RowIterator;
using Rows = Grid<T>::Rows;
using ConstRows = Grid<T>::ConstRows;

TEST_CLASS(Grid_RowTest) {
	TEST_METHOD(Alias_ValueType_IsType) {
		Assert::IsTrue(std::is_same_v<Row::value_type, int>);
	}

	TEST_METHOD(Alias_Pointer_IsTypePointer) {
		Assert::IsTrue(std::is_same_v<Row::pointer, Row::value_type*>);
	}

	TEST_METHOD(Alias_ConstPointer_IsTypeConstPointer) {
		Assert::IsTrue(std::is_same_v<Row::const_pointer, Row::value_type const*>);
	}

	TEST_METHOD(Alias_Reference_IsTypeReference) {
		Assert::IsTrue(std::is_same_v<Row::reference, Row::value_type&>);
	}

	TEST_METHOD(Alias_ConstReference_IsTypeConstReference) {
		Assert::IsTrue(std::is_same_v<Row::const_reference, Row::value_type const&>);
	}

	TEST_METHOD(Alias_Iterator_IsTypePointer) {
		Assert::IsTrue(std::is_same_v<Row::iterator, Row::pointer>);
	}

	TEST_METHOD(Alias_ConstIterator_IsTypeConstPointer) {
		Assert::IsTrue(std::is_same_v<Row::const_iterator, Row::const_pointer>);
	}

	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Row>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Row>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Row>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Row>);
	}

	TEST_METHOD(IsDefaultConstructible) {
		Assert::IsTrue(std::is_default_constructible_v<Row>);
	}

	TEST_METHOD(EnumerateUsingRangeFor_EnumerateEachValue) {
		int data[]{1, 2, 3};
		Row row{data, data + 3};
		int i = 0;

		for(int& it : row)
			Assert::AreEqual(++i, it);
	}

	TEST_METHOD(EnumerateUsingRangeForConst_EnumerateEachValue) {
		int data[]{1, 2, 3};
		Row row{data, data + 3};
		Row const const_row = std::as_const(row);
		int i = 0;

		for(int const& it : const_row)
			Assert::AreEqual(++i, it);
	}

	TEST_METHOD(Size_ReturnSize) {
		int data[]{1, 2, 3};
		Row const const_row{data, data + 3};
		Assert::AreEqual<size_t>(3, const_row.size());
	}

	TEST_METHOD(OperatorSubscript_ReturnValue) {
		int data[]{1, 2, 3};
		Row const const_row{data, data + 3};
		Assert::AreEqual(2, const_row[1]);
	}

	TEST_METHOD(OperatorSubscript_WriteToIt_Compiles) {
		int data[]{1, 2, 3};
		Row row{data, data + 3};
		row[1] = 5;
	}

	TEST_METHOD(OperatorSubscriptConst_ReturnValue) {
		int data[]{1, 2, 3};
		Row row{data, data + 3};
		Assert::AreEqual(2, row[1]);
	}

	TEST_METHOD(OperatorComparison_WhenBeginEndMismatch_ReturnFalse) {
		int data1[]{1, 2};
		int data2[]{1, 2};
		Row row1{data1, data1 + 2};
		Row Row{data2, data2 + 2};

		bool actual = row1 == Row;

		Assert::IsFalse(actual);
	}

	TEST_METHOD(OperatorComparison_WhenBeginEndMatch_ReturnTrue) {
		int data[]{1, 2};
		Row row1{data, data + 2};
		Row Row{data, data + 2};

		bool actual = row1 == Row;

		Assert::IsTrue(actual);
	}
};

TEST_CLASS(Grid_ConstRowIteratorTest) {
	TEST_METHOD(Alias_ValueType_IsType) {
		Assert::IsTrue(std::is_same_v<ConstRowIterator::value_type, Row>);
	}

	TEST_METHOD(Alias_Reference_IsTypeReference) {
		Assert::IsTrue(std::is_same_v<ConstRowIterator::reference, ConstRowIterator::value_type const&>);
	}

	TEST_METHOD(Alias_Pointer_IsTypePointer) {
		Assert::IsTrue(std::is_same_v<ConstRowIterator::pointer, ConstRowIterator::value_type const*>);
	}

	TEST_METHOD(Alias_DifferenceType_IsPtrDiffT) {
		Assert::IsTrue(std::is_same_v<ConstRowIterator::difference_type, ptrdiff_t>);
	}

	TEST_METHOD(Alias_IteratorCategory_IsRandomAccessIterator) {
		Assert::IsTrue(std::is_same_v<ConstRowIterator::iterator_category, std::random_access_iterator_tag>);
	}

	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<ConstRowIterator>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<ConstRowIterator>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<ConstRowIterator>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<ConstRowIterator>);
	}

	TEST_METHOD(IsDefaultConstructible) {
		Assert::IsTrue(std::is_default_constructible_v<ConstRowIterator>);
	}

	TEST_METHOD(IsDestructible) {
		Assert::IsTrue(std::is_destructible_v<ConstRowIterator>);
	}

	TEST_METHOD(IsSwappable) {
		Assert::IsTrue(std::is_swappable_v<ConstRowIterator>);
	}

	TEST_METHOD(OperatorSubscript_ReturnSpecifiedRow) {
		int data[]{1, 2, 3};
		ConstRowIterator c_row_it{data, 3};

		Row actual = c_row_it[1];

		Assert::AreEqual(1, actual[0]);
	}

	TEST_METHOD(OperatorIndirection_ReturnCurrentRow) {
		int data[]{1, 2, 3};
		ConstRowIterator c_row_it{data, 3};

		Row const actual = *c_row_it;

		Assert::AreEqual(1, *actual.begin());
	}

	TEST_METHOD(OperatorDereference_ReturnCurrentRow) {
		int data[]{1, 2, 3};
		ConstRowIterator c_row_it{data, 3};

		ConstRowIterator::pointer actual = c_row_it.operator->();

		Assert::AreEqual(1, *actual->begin());
	}

	TEST_METHOD(OperatorIncrementPrefix_MoveToNextRowAndReturnIt) {
		int data[]{1, 2, 3, 4};
		ConstRowIterator c_row_it{data, 2};

		ConstRowIterator& actual = ++c_row_it;

		Assert::AreEqual(3, *actual->begin());
	}

	TEST_METHOD(OperatorIncrementPostfix_MoveToNextRowAndReturnOriginalRow) {
		int data[]{1, 2, 3, 4};
		ConstRowIterator c_row_it{data, 2};

		ConstRowIterator actual = c_row_it++;

		Assert::AreEqual(1, *(*actual).begin());
		Assert::AreEqual(3, *c_row_it->begin());
	}

	TEST_METHOD(OperatorIncrementPrefix_WhenUsedOnACopy_DoNotIncrementSource) {
		int data[]{1, 2, 3, 4};
		auto c_row_it = ConstRowIterator{data, 2};
		auto c_row_it_copy = c_row_it;

		++c_row_it_copy;

		Assert::AreEqual(1, *c_row_it->begin());
		Assert::AreEqual(3, *c_row_it_copy->begin());
	}

	TEST_METHOD(OperatorDecrementPrefix_MoveToPreviousRowAndReturnIt) {
		int data[]{1, 2, 3, 4, 5, 6};
		ConstRowIterator c_row_it{data, 2};
		++++c_row_it;

		ConstRowIterator& actual = --c_row_it;

		Assert::AreEqual(3, *actual->begin());
	}

	TEST_METHOD(OperatorDecrementPostfix_MoveToPreviousRowAndReturnOriginalRow) {
		int data[]{1, 2, 3, 4, 5, 6};
		ConstRowIterator c_row_it{data, 2};
		++++c_row_it;

		ConstRowIterator actual = c_row_it--;

		Assert::AreEqual(5, *(*actual).begin());
		Assert::AreEqual(3, *c_row_it->begin());
	}

	TEST_METHOD(OperatorComparison_WhenReferencingOtherRow_ReturnFalse) {
		int data1[]{1, 2};
		int data2[]{1, 2};
		ConstRowIterator c_row_it1{data1, 2};
		ConstRowIterator c_row_it2{data2, 2};

		bool actual = c_row_it1 == c_row_it2;

		Assert::IsFalse(actual);
	}

	TEST_METHOD(OperatorComparison_WhenReferencingSameRowMatch_ReturnTrue) {
		int data[]{1, 2};
		ConstRowIterator c_row_it1{data, 2};
		ConstRowIterator c_row_it2{data, 2};

		bool actual = c_row_it1 == c_row_it2;

		Assert::IsTrue(actual);
	}
};

TEST_CLASS(Grid_RowIteratorTest) {
	TEST_METHOD(Alias_ValueType_IsType) {
		Assert::IsTrue(std::is_same_v<RowIterator::value_type, Row>);
	}

	TEST_METHOD(Alias_Reference_IsTypeReference) {
		Assert::IsTrue(std::is_same_v<RowIterator::reference, RowIterator::value_type&>);
	}

	TEST_METHOD(Alias_Pointer_IsTypePointer) {
		Assert::IsTrue(std::is_same_v<RowIterator::pointer, RowIterator::value_type*>);
	}

	TEST_METHOD(Alias_DifferenceType_IsPtrDiffT) {
		Assert::IsTrue(std::is_same_v<RowIterator::difference_type, ptrdiff_t>);
	}

	TEST_METHOD(Alias_IteratorCategory_IsRandomAccessIterator) {
		Assert::IsTrue(std::is_same_v<RowIterator::iterator_category, std::random_access_iterator_tag>);
	}

	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<RowIterator>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<RowIterator>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<RowIterator>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<RowIterator>);
	}

	TEST_METHOD(IsDefaultConstructible) {
		Assert::IsTrue(std::is_default_constructible_v<RowIterator>);
	}

	TEST_METHOD(IsDestructible) {
		Assert::IsTrue(std::is_destructible_v<RowIterator>);
	}

	TEST_METHOD(IsSwappable) {
		Assert::IsTrue(std::is_swappable_v<RowIterator>);
	}

	TEST_METHOD(OperatorSubscript_ReturnSpecifiedRow) {
		int data[]{1, 2, 3};
		RowIterator row_it{data, 3};

		Row actual = row_it[1];

		Assert::AreEqual(1, actual[0]);
	}

	TEST_METHOD(OperatorIndirection_ReturnCurrentRow) {
		int data[]{1, 2, 3};
		RowIterator row_it{data, 3};
		Row& actual = *row_it;
	}

	TEST_METHOD(OperatorDereference_ReturnCurrentRow) {
		int data[]{1, 2, 3};
		RowIterator row_it{data, 3};

		Row* actual = row_it.operator->();

		Assert::AreEqual(1, *actual->begin());
	}

	TEST_METHOD(OperatorIncrementPrefix_MoveToNextRowAndReturnIt) {
		int data[]{1, 2, 3, 4};
		RowIterator row_it{data, 2};

		RowIterator& actual = ++row_it;

		Assert::AreEqual(3, *actual->begin());
	}

	TEST_METHOD(OperatorIncrementPostfix_MoveToNextRowAndReturnOriginalRow) {
		int data[]{1, 2, 3, 4};
		RowIterator row_it{data, 2};

		RowIterator actual = row_it++;

		Assert::AreEqual(1, *(*actual).begin());
		Assert::AreEqual(3, *row_it->begin());
	}

	TEST_METHOD(OperatorIncrementPrefix_WhenUsedOnACopy_DoNotIncrementSource) {
		int data[]{1, 2, 3, 4};
		auto row_it = RowIterator{data, 2};
		auto row_it_copy = row_it;

		++row_it_copy;

		Assert::AreEqual(1, *row_it->begin());
		Assert::AreEqual(3, *row_it_copy->begin());
	}

	TEST_METHOD(OperatorDecrementPrefix_MoveToPreviousRowAndReturnIt) {
		int data[]{1, 2, 3, 4, 5, 6};
		RowIterator row_it{data, 2};
		++++row_it;

		RowIterator& actual = --row_it;

		Assert::AreEqual(3, *actual->begin());
	}

	TEST_METHOD(OperatorDecrementPostfix_MoveToPreviousRowAndReturnOriginalRow) {
		int data[]{1, 2, 3, 4, 5, 6};
		RowIterator row_it{data, 2};
		++++row_it;

		RowIterator actual = row_it--;

		Assert::AreEqual(5, *(*actual).begin());
		Assert::AreEqual(3, *row_it->begin());
	}

	TEST_METHOD(OperatorComparison_WhenReferencingOtherRow_ReturnFalse) {
		int data1[]{1, 2};
		int data2[]{1, 2};
		RowIterator row_it1{data1, 2};
		RowIterator row_it2{data2, 2};

		bool actual = row_it1 == row_it2;

		Assert::IsFalse(actual);
	}

	TEST_METHOD(OperatorComparison_WhenReferencingSameRowMatch_ReturnTrue) {
		int data[]{1, 2};
		RowIterator row_it1{data, 2};
		RowIterator row_it2{data, 2};

		bool actual = row_it1 == row_it2;

		Assert::IsTrue(actual);
	}
};

TEST_CLASS(Grid_RowsTest) {
	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Rows>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Rows>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Rows>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Rows>);
	}

	TEST_METHOD(IsDefaultConstructible) {
		Assert::IsTrue(std::is_default_constructible_v<Rows>);
	}

	TEST_METHOD(EnumerateUsingRangeFor_EnumerateEachRow) {
		//Arrange
		constexpr auto kWidth = 3;
		constexpr auto kHeight = 2;
		constexpr auto kSize = kWidth * kHeight;
		auto test_data = assist::CreateIntArray<kSize>();
		Rows rows{test_data.data(), test_data.data() + kSize, kWidth};
		assist::GridVectorAsserter grid_vec{kWidth, kHeight};

		//Act
		for(int y{}; Row & row : rows) {
			for(int& cell : row)
				grid_vec.Push(y, cell);
			++y;
		}

		//Assert
		grid_vec.AssertSameAsArray(test_data);
	}

	TEST_METHOD(EnumerateUsingRangeForConst_EnumerateEachRow) {
		//Arrange
		constexpr auto kWidth = 3;
		constexpr auto kHeight = 2;
		constexpr auto kSize = kWidth * kHeight;
		auto test_data = assist::CreateIntArray<kSize>();
		ConstRows const_rows{test_data.data(), test_data.data() + kSize, kWidth};
		assist::GridVectorAsserter grid_vec{kWidth, kHeight};

		//Act
		for(int y{}; Row const& row : const_rows) {
			for(int const& cell : row)
				grid_vec.Push(y, cell);
			++y;
		}

		//Assert
		grid_vec.AssertSameAsArray(test_data);
	}

	TEST_METHOD(EnumerateUsingBeginEnd_EnumerateEachRow) {
		//Arrange
		constexpr auto kWidth = 3;
		constexpr auto kHeight = 2;
		constexpr auto kSize = kWidth * kHeight;
		auto test_data = assist::CreateIntArray<kSize>();
		auto rows = Rows{test_data.data(), test_data.data() + kSize, kWidth};
		assist::GridVectorAsserter grid_vec{kWidth, kHeight};

		//Act
		RowIterator row_begin = rows.begin();
		RowIterator row_end = rows.end();
		RowIterator row_it = row_begin;
		auto y = int{};
		while(row_it != row_end) {
			Row& row = row_it.operator*();
			int* cell_begin = row.begin();
			int* cell_end = row.end();
			int* cell_it = cell_begin;
			while(cell_it != cell_end) {
				int& cell = *cell_it;
				grid_vec.Push(y, cell);
				++cell_it;
			}
			++row_it, ++y;
		}

		//Assert
		grid_vec.AssertSameAsArray(test_data);
	}
};

TEST_CLASS(Grid_GridTest) {
public:
	TEST_METHOD(Alias_ValueType_IsType) {
		Assert::IsTrue(std::is_same_v<Grid<T>::value_type, int>);
	}

	TEST_METHOD(Alias_Reference_IsTypeReference) {
		Assert::IsTrue(std::is_same_v<Grid<T>::reference, int&>);
	}

	TEST_METHOD(Alias_ConstReference_IsConstTypeRef) {
		Assert::IsTrue(std::is_same_v<Grid<T>::const_reference, int const&>);
	}

	TEST_METHOD(Alias_Iterator_IsVectorTypeIterator) {
		Assert::IsTrue(std::is_same_v<Grid<T>::iterator, std::vector<int>::iterator>);
	}

	TEST_METHOD(Alias_ConstIterator_IsVectorTypeConstIterator) {
		Assert::IsTrue(std::is_same_v<Grid<T>::const_iterator, std::vector<int>::const_iterator>);
	}

	TEST_METHOD(Alias_DifferenceType_IsVectorTypeDifferenceType) {
		Assert::IsTrue(std::is_same_v<Grid<T>::difference_type, std::vector<int>::difference_type>);
	}

	TEST_METHOD(Alias_SizeType_IsVectorTypeSizeType) {
		Assert::IsTrue(std::is_same_v<Grid<T>::size_type, std::vector<int>::size_type>);
	}

	TEST_METHOD(IsDefaultConstructible) {
		Assert::IsTrue(std::is_default_constructible_v<Grid<T>>);
	}

	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Grid<T>>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Grid<T>>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Grid<T>>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Grid<T>>);
	}

	TEST_METHOD(IsDestructible) {
		Assert::IsTrue(std::is_destructible_v<Grid<T>>);
	}

	TEST_METHOD(Width_ReturnWidth) {
		Grid<T> grid(5, 3);
		Assert::AreEqual<size_t>(5, grid.width());
	}

	TEST_METHOD(Height_ReturnHeight) {
		Grid<T> grid(5, 3);
		Assert::AreEqual<size_t>(3, grid.height());
	}

	TEST_METHOD(Size_ReturnWidthTimesHeight) {
		Grid<T> grid(5, 3);
		Assert::AreEqual<size_t>(15, grid.size());
	}

	TEST_METHOD(MaxSize_GreaterThanOne) {
		Grid<T> grid(5, 3);

		Grid<T>::size_type actual = grid.max_size();

		Assert::IsTrue(actual > 0);
	}

	TEST_METHOD(Data_ReturnSameAsBegin) {
		Grid<T> grid(5, 3);
		Assert::IsTrue(*grid.data() == *grid.begin());
	}

	TEST_METHOD(DataConst_ReturnSameAsBegin) {
		Grid<T> grid(5, 3);
		Grid<T> const& const_grid = std::as_const(grid);
		Assert::IsTrue(*const_grid.data() == *const_grid.begin());
	}

	TEST_METHOD(Empty_GridIsEmpty_ReturnTrue) {
		Grid<T> grid;
		Assert::IsTrue(grid.empty());
	}

	TEST_METHOD(Empty_GridIsNotEmpty_ReturnFalse) {
		Grid<T> grid(3, 3);
		Assert::IsFalse(grid.empty());
	}

	TEST_METHOD(OperatorSubscript_ReturnValue) {
		Grid<T> grid(2, 2);
		*(grid.data() + 2) = 53;

		int& actual = grid[2];

		Assert::AreEqual(53, actual);
	}

	TEST_METHOD(OperatorSubscript_WriteToIt_Compiles) {
		Grid<T> grid(2, 2);
		grid[2] = 50;
		Assert::AreEqual(50, grid[2]);
	}

	TEST_METHOD(OperatorSubscriptConst_ReturnValue) {
		Grid<T> grid(2, 2);
		*(grid.data() + 2) = 53;
		Grid<T> const& const_grid = std::as_const(grid);

		int const& actual = const_grid[2];

		Assert::AreEqual(53, actual);
	}

	TEST_METHOD(BeginEnd_DifferenceIsSameAsSize) {
		Grid<T> grid(5, 3);
		ptrdiff_t diff = grid.end() - grid.begin();
		Assert::IsTrue(diff == grid.size());
	}

	TEST_METHOD(EnumerateUsingRangeFor_EnumerateEachValue) {
		auto grid = assist::CreateFilledGrid<T>(5, 3);
		int i = 0;

		for(int& it : grid)
			Assert::AreEqual(++i, it);
	}

	TEST_METHOD(EnumerateUsingRangeForConst_EnumerateEachValue) {
		auto grid = assist::CreateFilledGrid<T>(5, 3);
		int i = 0;

		for(int const& it : std::as_const(grid))
			Assert::AreEqual(++i, it);
	}

	TEST_METHOD(OperatorComparison_WhenSizeMismatch_ReturnFalse) {
		auto a = assist::CreateFilledGrid<T>(5, 3);
		auto b = assist::CreateFilledGrid<T>(3, 5);

		Assert::IsFalse(a == b);
		Assert::IsTrue(a != b);
	}

	TEST_METHOD(OperatorComparison_WhenDataMismatch_ReturnFalse) {
		auto a = assist::CreateFilledGrid<T>(2, 3);
		auto b = Grid<T>(2, 3);

		Assert::IsFalse(a == b);
		Assert::IsTrue(a != b);
	}

	TEST_METHOD(OperatorComparison_WhenDataMatch_ReturnTrue) {
		auto a = assist::CreateFilledGrid<T>(2, 3);
		auto b = assist::CreateFilledGrid<T>(2, 3);

		Assert::IsTrue(a == b);
		Assert::IsFalse(a != b);
	}

	TEST_METHOD(Swap_MemberFunction_Swapped) {
		Grid<T> a(2, 3);
		Grid<T> b(3, 2);

		a.swap(b);

		Assert::AreEqual<size_t>(3, a.width());
		Assert::AreEqual<size_t>(2, b.width());
	}

	TEST_METHOD(Swap_GlobalFunction_Swapped) {
		Grid<T> a(2, 3);
		Grid<T> b(3, 2);

		swap(a, b);

		Assert::AreEqual<size_t>(3, a.width());
		Assert::AreEqual<size_t>(2, b.width());
	}

	TEST_METHOD(Initializer_Width_ReturnWidth) {
		Grid<T> grid{1, 2, {}};
		Assert::AreEqual<size_t>(1, grid.width());
	}

	TEST_METHOD(Initializer_Height_ReturnHeight) {
		Grid<T> grid{1, 2, {}};
		Assert::AreEqual<size_t>(2, grid.height());
	}

	TEST_METHOD(Initializer_Empty_AllItemsHaveDefaultValue) {
		Grid<T> grid{1, 2, {}};
		Assert::IsTrue(std::all_of(grid.begin(), grid.end(), [](int i) { return i == 0; }));
	}

	TEST_METHOD(Initializer_AllItemsInitialized) {
		auto values = {4, 3, 2, 6, 4, 1};

		Grid<T> grid{3, 2, values};

		Assert::IsTrue(std::equal(grid.begin(), grid.end(), values.begin()));
	}

	TEST_METHOD(Initializer_TooManyItems_ExtraItemsIgnored) {
		Grid<T> grid{1, 2, {4, 5, 6, 7}};

		auto expected = {4, 5, 6};
		Assert::IsTrue(std::equal(grid.begin(), grid.end(), expected.begin()));
	}

	TEST_METHOD(Initializer_NotEnoughItems_RemainingItemsHaveDefaultValue) {
		Grid<T> grid{1, 2, {1, 2}};

		auto expected = {1, 2, 0, 0};
		Assert::IsTrue(std::equal(grid.begin(), grid.end(), expected.begin()));
	}

	TEST_METHOD(EnumerateRows_EnumerateUsingRangeFor_EnumerateEachRow) {
		//Arrange
		constexpr auto kWidth = 3;
		constexpr auto kHeight = 2;
		auto test_data = assist::CreateIntArray<kWidth * kHeight>();
		Grid<T> grid = assist::CreateFilledGrid<T>(kWidth, kHeight);
		assist::GridVectorAsserter grid_vec{kWidth, kHeight};

		//Act
		for(int y{}; Row & row : grid.EnumerateRows()) {
			for(int& cell : row)
				grid_vec.Push(y, cell);
			++y;
		}

		//Assert
		grid_vec.AssertSameAsArray(test_data);
	}

	TEST_METHOD(EnumerateRows_EnumerateUsingRangeForConst_EnumerateEachRow) {
		//Arrange
		constexpr auto kWidth = 3;
		constexpr auto kHeight = 2;
		auto test_data = assist::CreateIntArray<kWidth * kHeight>();
		Grid<T> const const_grid = assist::CreateFilledGrid<T>(kWidth, kHeight);
		assist::GridVectorAsserter grid_vec{kWidth, kHeight};

		//Act
		for(int y{}; auto&& row : const_grid.EnumerateRows()) {
			for(auto&& cell : row)
				grid_vec.Push(y, cell);
			++y;
		}

		//Assert
		grid_vec.AssertSameAsArray(test_data);
	}
};
