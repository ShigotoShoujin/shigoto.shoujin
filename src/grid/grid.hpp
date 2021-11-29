#pragma once
#include "iterable.hpp"
#include "iterable_iterator.hpp"
#include <vector>

namespace shoujin {

template<typename T>
class Grid : public Iterable<T> {
public:
	Grid(int width, int height);

	[[nodiscard]] int width() const { return _width; }
	[[nodiscard]] int height() const { return _height; }
	[[nodiscard]] size_t size() const { return _data.size(); }
	[[nodiscard]] T* data() const { return _data.data(); }
	[[nodiscard]] T& operator[](int index) const { return _data[index]; }
	[[nodiscard]] T* data() { return _data.data(); }
	[[nodiscard]] T& operator[](int index) { return _data[index]; }
	//Iterable<IterableIterator<T>> Rows();

private:
	int _width, _height;
	std::vector<T> _data;
};

template<typename T>
Grid<T>::Grid(int width, int height) :
	_width{width},
	_height{height}
{
	_data.resize(width * height);
	Iterable<T>::SetBeginEnd(_data.data(), _data.data() + _data.size());
}

//template<typename T>
//Iterable<IterableIterator<T>> Grid<T>::Rows()
//{
//}

}
