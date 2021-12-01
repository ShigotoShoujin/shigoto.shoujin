#include "CppUnitTest.h"
#include <array>
#include <cassert>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace assist {

class GridVectorAsserter {
public:
	GridVectorAsserter(int width, int height) :
		_width{width},
		_height{height},
		_expected{height}
	{
		assert(_height > 0);
	}

	void Push(int row, int value)
	{
		_expected[row].push_back(value);
	}

	template<int Size>
	void AssertSameAsArray(const std::array<int, Size> array)
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
	void _assert_match_array(const std::array<int, Size> array)
	{
		for(int i = 0; i < _height; ++i)
			Assert::IsTrue(_compare_row(_expected[i].data(), array.data() + i * _width));
	}

	bool _compare_row(const int* actual, const int* expected)
	{
		bool result = false;
		for(int i = 0; i < _width; ++i, ++actual, ++expected)
			result |= *actual == *expected;
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

}
