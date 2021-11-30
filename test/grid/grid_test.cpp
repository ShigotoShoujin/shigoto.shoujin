//#include "CppUnitTest.h"
//
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//
//#include "../../src/grid/grid.hpp"
//#include <array>
//#include <vector>
//
//using namespace shoujin;
//
//TEST_CLASS(GridTest) {
//public:
//	TEST_METHOD(IsCopyConstructible) {
//		Assert::IsTrue(std::is_copy_constructible_v<Grid<int>>);
//	}
//
//	TEST_METHOD(IsCopyAssignable) {
//		Assert::IsTrue(std::is_copy_assignable_v<Grid<int>>);
//	}
//
//	TEST_METHOD(IsMoveConstructible) {
//		Assert::IsTrue(std::is_move_constructible_v<Grid<int>>);
//	}
//
//	TEST_METHOD(IsMoveAssignable) {
//		Assert::IsTrue(std::is_move_assignable_v<Grid<int>>);
//	}
//
//	TEST_METHOD(Ctor_WidthHeightSizeInitialized) {
//		//Arrange
//		const int kWidth = 2, kHeight = 3;
//
//		//Act
//		Grid<int> grid(kWidth, kHeight);
//
//		//Assert
//		Assert::AreEqual(kWidth, grid.width());
//		Assert::AreEqual(kHeight, grid.height());
//		Assert::AreEqual<size_t>(kWidth * kHeight, grid.size());
//	}
//
//	TEST_METHOD(LinearIterate_OK) {
//		//Arrange
//		Grid<int> grid(5, 4);
//		std::vector<int> result;
//
//		//Act
//		for(auto&& it : grid)
//			result.push_back(it);
//
//		//Assert
//		Assert::AreEqual(grid.size(), result.size());
//		size_t end = grid.size();
//		for(int i = 0; i < end; ++i)
//			Assert::AreEqual(grid[i], result[i]);
//	}
//
//	TEST_METHOD(RowIterateRangeFor_OK) {
//		//Arrange
//		const int kWidth = 7, kHeight = 5;
//		auto grid = _make_grid(kWidth, kHeight);
//		std::vector<std::vector<int>> result(kHeight);
//
//		//Act
//		int y = 0;
//		for(auto&& row : grid.Rows()) {
//			for(auto&& cell : row) {
//				result[y].push_back(cell);
//			}
//		}
//	}
//
//	TEST_METHOD(RowIterateBeginEnd_OK) {
//		//Arrange
//		const int kWidth = 7, kHeight = 5;
//		auto grid = _make_grid(kWidth, kHeight);
//		std::vector<std::vector<int>> result(kHeight);
//
//		//Act
//		int y = 0;
//
//		auto&& row_begin = grid.Rows().begin();
//		auto&& row_end = grid.Rows().end();
//		auto&& row_it = row_begin;
//		while(row_it != row_end) {
//			auto&& cell_begin = row_it->begin();
//			auto&& cell_end = row_it->end();
//			auto&& cell_it = cell_begin;
//			while(cell_it != cell_end) {
//				result[y].push_back(*cell_it);
//				++cell_it;
//			}
//			++row_it;
//		}
//	}
//
//	Grid<int> _make_grid(int width, int height)
//	{
//		Grid<int> grid(width, height);
//		for(int i = 0; auto&& it : grid)
//			it = ++i;
//		return grid;
//	}
//};
