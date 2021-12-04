#pragma once
#include "iterable.hpp"
#include <vector>

namespace shoujin {

template<typename T>
class Grid : public Iterable<T*> {
public:
#include "grid.inc"

	Grid(int width, int height) :
		_width{width},
		_height{height}
	{
		_construct();
	}

	template<int width, int height>
	static Grid<T> CreateFromArray(const std::array<T, width * height>& array)
	{
		Grid<T> grid(width, height);
		auto* data = grid.data();
		for(int i = 0, size = width * height; i < size; ++i)
			data[i] = array[i];
		return grid;
	}

	[[nodiscard]] int width() const { return _width; }
	[[nodiscard]] int height() const { return _height; }
	[[nodiscard]] size_t size() const { return _data.size(); }
	[[nodiscard]] T* data() const { return _data.data(); }
	[[nodiscard]] T& operator[](int index) const { return _data[index]; }
	[[nodiscard]] T* data() { return _data.data(); }
	[[nodiscard]] T& operator[](int index) { return _data[index]; }

	Iterable<RowsIterator<T>> Rows()
	{
		T* begin = _data.data();
		T* end = begin + _data.size();
		return Iterable<RowsIterator<T>>{{begin, _width}, {end, _width}};
	}

private:
	int _width, _height;
	std::vector<T> _data;

	void _construct()
	{
		_data.resize(_width * _height);
		Iterable<T*>::Reset(_data.data(), _data.data() + _data.size());
	}
};

}
