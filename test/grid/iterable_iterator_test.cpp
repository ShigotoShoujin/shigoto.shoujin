#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../src/grid/iterable_iterator.hpp"
#include <array>
#include <vector>

using namespace shoujin;

TEST_CLASS(IterableIteratorTest) {
public:
	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<IterableIterator<int>>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<IterableIterator<int>>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<IterableIterator<int>>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<IterableIterator<int>>);
	}

	TEST_METHOD(IterateGrid_OK) {
		//Arrange
		const int width = 5, height = 6;
		auto arr = _array_of_int<width * height>();
		IterableIterator<int> ii{arr.data(), arr.data() + arr.size(), width};
		std::vector<std::vector<int>> result(height);

		//Act
		for(int y = 0; y < height; ++y) {
			for(auto&& it : ii)
				result[y].push_back(it);
			++ii;
		}

		//Assert
		auto compare_row = [&width](int* actual, int* expected) -> bool {
			bool result = false;
			for(int i = 0; i < width; ++i, ++actual, ++expected)
				result |= *actual == *expected;
			return result;
		};

		Assert::AreEqual<size_t>(height, result.size());
		for(int i = 0; i < height; ++i)
			Assert::IsTrue(compare_row(result[i].data(), arr.data() + i * width));
	}

private:
	template<int Size>
	std::array<int, Size> _array_of_int()
	{
		std::array<int, Size> arr;
		for(int i = 0; i < Size; ++i)
			arr[i] = i + 1;
		return arr;
	}
};
