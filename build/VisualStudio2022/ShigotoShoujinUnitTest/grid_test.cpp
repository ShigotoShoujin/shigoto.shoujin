#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../ShigotoShoujin/grid_wip.hpp"

TEST_CLASS(GridTest) {
public:
	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Grid>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Grid>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Grid>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Grid>);
	}

	TEST_METHOD(Grid_WIP_Test)
	{
		Grid grid(8, 8);
		IterateWithFor(grid);
		IterateWithRangeFor(grid);
	}
};
