#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../../src/grid.hpp"

using namespace shoujin;

#include <iostream>
#include <iomanip>
static void IterateUsingRangeFor(Grid<int>& grid)
{
	std::cout << "Iterate though all rows cells\n";
	for(auto&& row_it : grid.Rows()) {
		//for(auto&& cell : row_it)
		//	std::cout << std::setw(2) << std::setfill('0') << cell << ',';
		std::cout << '\n';
	}
	std::cout << '\n';
}

TEST_CLASS(GridTest) {
public:
	TEST_METHOD(IsNotCopyConstructible) {
		Assert::IsFalse(std::is_copy_constructible_v<Grid<int>>);
	}

	TEST_METHOD(IsNotCopyAssignable) {
		Assert::IsFalse(std::is_copy_assignable_v<Grid<int>>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Grid<int>>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Grid<int>>);
	}

	TEST_METHOD(Grid_WIP_Test)
	{
		Grid<int> grid(8, 5);
		//IterateWithFor(grid);
		IterateUsingRangeFor(grid);
	}
};
