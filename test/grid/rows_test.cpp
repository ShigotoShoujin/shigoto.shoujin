#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../src/grid/rows.hpp"
#include "assist/grid_assist.hpp"

using namespace shoujin;

TEST_CLASS(RowsTest) {
public:
	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Rows<int>>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Rows<int>>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Rows<int>>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Rows<int>>);
	}

	TEST_METHOD(IterateRangeFor_OK) {
		//Arrange
		const int width = 5, height = 6;
		constexpr int array_size = width * height;
		auto arr = assist::CreateIntArray<array_size>();
		Rows<int> rows{arr.data(), arr.data() + arr.size(), width};
		assist::GridVectorAsserter grid_vec{width, height};

		//Act
		for(int y = 0; auto&& row : rows) {
			for(auto&& cell : row)
				grid_vec.Push(y, cell);
			++y;
		}

		//Assert
		grid_vec.AssertSameAsArray<array_size>(arr);
	}

	TEST_METHOD(IterateBeginEnd_OK) {
		//Arrange
		const int width = 5, height = 6;
		constexpr int array_size = width * height;
		auto arr = assist::CreateIntArray<array_size>();
		Rows<int> rows{arr.data(), arr.data() + arr.size(), width};
		assist::GridVectorAsserter grid_vec{width, height};

		//Act
		int y{};
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
				++cell_it;
			}
			++row_it;
			++y;
		}

		//Assert
		grid_vec.AssertSameAsArray<array_size>(arr);
	}
};
