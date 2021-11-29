#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../src/grid/grid.hpp"
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
		for(int i = 0, end = grid.size(); i < end; ++i)
			Assert::AreEqual(grid[i], result[i]);
	}
};
