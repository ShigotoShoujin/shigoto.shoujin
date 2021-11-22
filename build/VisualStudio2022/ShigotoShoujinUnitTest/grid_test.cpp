#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../../src/grid.hpp"

using namespace shoujin;

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
		Grid<int> grid(8, 8);
		//IterateWithFor(grid);
		//IterateWithRangeFor(grid);
	}
};
