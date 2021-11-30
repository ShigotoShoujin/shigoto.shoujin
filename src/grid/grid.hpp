#pragma once
#include "iterable.hpp"
#include "iterable_iterator.hpp"
#include <vector>

namespace shoujin {

template<typename T>
class Grid : public Iterable<T*> {
public:
	Grid(int width, int height);

	[[nodiscard]] int width() const { return _width; }
	[[nodiscard]] int height() const { return _height; }
	[[nodiscard]] size_t size() const { return _data.size(); }
	[[nodiscard]] T* data() const { return _data.data(); }
	[[nodiscard]] T& operator[](int index) const { return _data[index]; }
	[[nodiscard]] T* data() { return _data.data(); }
	[[nodiscard]] T& operator[](int index) { return _data[index]; }
	//Iterable<IterableIterator<ForwardStepIterator<T>, ForwardStepIterator<T>>*> Rows();

private:
	int _width, _height;
	std::vector<T> _data;
	IterableIterator<ForwardStepIterator<T>> _iterableiterator_begin, _iterableiterator_end;
	ForwardStepIterator<T> a, b, c, d;
};

template<typename T>
Grid<T>::Grid(int width, int height) :
	_width{width},
	_height{height}
{
	_data.resize(width * height);
	Iterable<T*>::SetBeginEnd(_data.data(), _data.data() + _data.size());

	auto* begin = data();
	auto* end = data() + size();
	a = {begin, 1};
	b = {begin + _width, 1};
	c = {end, 1};
	d = {end, 1};

	_iterableiterator_begin = {&a, &b, 1};
	_iterableiterator_end = {&c, &d, 1};
}
//
//template<typename T>
//Iterable<IterableIterator<ForwardStepIterator<T>, ForwardStepIterator<T>>*> Grid<T>::Rows()
//{
//	return { &_iterableiterator_begin, &_iterableiterator_end };
//}

}
