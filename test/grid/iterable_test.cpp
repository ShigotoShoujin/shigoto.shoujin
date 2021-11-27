#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../src/grid/iterable.hpp"
#include <array>
#include <vector>

using namespace shoujin;

TEST_CLASS(IterableTest) {
public:
	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Iterable<int>>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Iterable<int>>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Iterable<int>>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Iterable<int>>);
	}

	TEST_METHOD(IterateOver5_OK) {
		//Arrange
		auto arr = _array_of_int<5>();
		auto iterable = Iterable<int>{arr.data(), arr.data() + arr.size()};
		std::vector<int> result;
		result.reserve(5);

		//Act
		for(auto&& it : iterable)
			result.push_back(it);

		//Assert
		Assert::AreEqual<size_t>(5, result.size());
		for(int i = 0; i < 5; ++i)
			Assert::AreEqual(i + 1, result[i]);
	}

	TEST_METHOD(ConstIterateOver5_OK) {
		//Arrange
		auto arr = _array_of_int<5>();
		const auto iterable = Iterable<int>{arr.data(), arr.data() + arr.size()};
		std::vector<int> result;
		result.reserve(5);

		//Act
		for(auto& it : iterable) {
			//it = 5; //Will not compile
			result.push_back(it);
		}

		//Assert
		Assert::AreEqual<size_t>(5, result.size());
		for(int i = 0; i < 5; ++i)
			Assert::AreEqual(i + 1, result[i]);
	}

	TEST_METHOD(SetBegin_OK) {
		//Arrange
		auto arr = _array_of_int<5>();
		auto iterable = Iterable<int>{arr.data(), arr.data() + 2};
		std::vector<int> result;
		result.reserve(5);

		//Act
		for(auto&& it : iterable)
			result.push_back(it);

		iterable.SetBegin(arr.data() + 3);

		for(auto&& it : iterable)
			result.push_back(it);

		//Assert
		Assert::AreEqual<size_t>(4, result.size());
		Assert::AreEqual(1, result[0]);
		Assert::AreEqual(2, result[1]);
		Assert::AreEqual(4, result[2]);
		Assert::AreEqual(5, result[3]);
	}

	TEST_METHOD(IterableGrid_OK) {
		//Arrange
		const int width = 8, height = 4;
		auto arr = _array_of_int<width * height>();
		auto iterable = Iterable<int>{arr.data(), arr.data() + width};
		std::vector<std::vector<int>> result(height);

		//Act
		for(int y = 0; y < height; ++y) {
			for(int i = 0; auto&& it : iterable)
				result[y].push_back(it);
			iterable.SetBegin(arr.data() + (y + 1) * width);
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
