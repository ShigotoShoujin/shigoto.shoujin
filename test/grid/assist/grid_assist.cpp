#include "grid_assist.hpp"
#include <cassert>

namespace assist {

GridVectorAsserter::GridVectorAsserter(int width, int height) :
	_width{width},
	_height{height},
	_expected{height}
{
	assert(_height > 0);
}

void GridVectorAsserter::Push(int row, int value)
{
	_expected[row].push_back(value);
}

void GridVectorAsserter::_assert_all_rows_same_size()
{
	auto size = _expected[0].size();
	for(auto& row : _expected)
		Assert::AreEqual(size, row.size());
}

bool GridVectorAsserter::_compare_row(const int* actual, const int* expected)
{
	bool result = true;
	for(int i = 0; i < _width; ++i, ++actual, ++expected)
		result &= *actual == *expected;
	return result;
}

}
