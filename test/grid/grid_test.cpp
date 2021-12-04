#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../src/grid/grid.hpp"
#include "assist/grid_assist.hpp"
#include <array>
#include <vector>

using namespace shoujin;

TEST_CLASS(GridTest) {
public:
	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Grid<int>>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Grid<int>>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Grid<int>>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Grid<int>>);
	}

	TEST_METHOD(Ctor_WidthHeightSizeInitialized) {
		//Arrange
		const int kWidth = 2, kHeight = 3;

		//Act
		Grid<int> grid(kWidth, kHeight);

		//Assert
		Assert::AreEqual(kWidth, grid.width());
		Assert::AreEqual(kHeight, grid.height());
		Assert::AreEqual<size_t>(kWidth * kHeight, grid.size());
	}

	TEST_METHOD(LinearIterate_OK) {
		//Arrange
		Grid<int> grid(5, 4);
		std::vector<int> result;

		//Act
		for(auto&& it : grid)
			result.push_back(it);

		//Assert
		Assert::AreEqual(grid.size(), result.size());
		size_t end = grid.size();
		for(int i = 0; i < end; ++i)
			Assert::AreEqual(grid[i], result[i]);
	}

	TEST_METHOD(IterateRangeFor_OK) {
		//Arrange
		constexpr int width = 5, height = 6;
		constexpr int array_size = width * height;
		auto arr = assist::CreateIntArray<array_size>();
		auto grid = Grid<int>::CreateFromArray<width, height>(arr);
		assist::GridVectorAsserter grid_vec{width, height};

		//Act
		for(int y = 0; auto&& row : grid.Rows()) {
			for(auto&& cell : row)
				grid_vec.Push(y, cell);
			++y;
		}

		//Assert
		grid_vec.AssertSameAsArray<array_size>(arr);
	}

	TEST_METHOD(IterateBeginEnd_OK) {
		//Arrange
		constexpr int width = 5, height = 6;
		constexpr int array_size = width * height;
		auto arr = assist::CreateIntArray<array_size>();
		auto grid = Grid<int>::CreateFromArray<width, height>(arr);
		assist::GridVectorAsserter grid_vec{width, height};

		//Act
		int y{};
		auto&& rows = grid.Rows();
		auto&& row_begin = rows.begin();
		auto&& row_end = rows.end();
		auto&& row_it = row_begin;
		while(row_it != row_end) {
			auto&& row = *row_begin;
			auto&& cell_begin = row.begin();
			auto&& cell_end = row.end();
			auto&& cell_it = cell_begin;
			while(cell_it != cell_end) {
				auto&& cell = *cell_begin;
				grid_vec.Push(y, cell);
				cell_it++;
			}
			row_it++;
			++y;
		}

		//Assert
		grid_vec.AssertSameAsArray<array_size>(arr);
	}

	Grid<int> _make_grid(int width, int height)
	{
		Grid<int> grid(width, height);
		for(int i = 0; auto&& it : grid)
			it = ++i;
		return grid;
	}
};
