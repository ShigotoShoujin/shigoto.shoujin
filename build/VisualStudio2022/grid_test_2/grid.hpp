#pragma once
#include "iterable.hpp"
#include "grid_rows.hpp"

template<typename T>
class Grid : public Iterable<T*>
{
public:
	Grid(int width, int height);
	Grid(const Grid&) = delete;
	Grid& operator=(const Grid&) = delete;
	Grid(Grid&&) = delete;
	Grid& operator=(Grid&&) = delete;
	~Grid();

	int width() const { return _width; }
	int height() const { return _height; }
	const GridRows<T> Rows() const;
	GridRows<T> Rows();

private:
	int _width, _height;
	T* _begin, * _end;
};

template<typename T>
Grid<T>::Grid(int width, int height) : _width{ width }, _height{ height }
{
	auto size = width * height;
	_begin = new T[size];
	_end = _begin + size;
	Iterable<T*>::_begin = _begin;
	Iterable<T*>::_end = _end;
}

template<typename T>
Grid<T>::~Grid()
{
	if (Iterable<T*>::_begin)
	{
		delete[] _begin;
		_begin = nullptr;
		_end = nullptr;
		_width = 0;
		_height = 0;
	}
}

template<typename T>
const GridRows<T> Grid<T>::Rows() const
{
	return GridRows<T>{_begin, _end, _width};
}

template<typename T>
GridRows<T> Grid<T>::Rows()
{
	return GridRows<T>{_begin, _end, _width};
}
