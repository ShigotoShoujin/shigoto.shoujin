#pragma once
#include <concepts>

namespace shoujin {

template<typename T>
concept Cell = std::default_initializable<T> && std::movable<T>;

template<Cell T>
class CellIterator {
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using reference = T&;

	CellIterator(T* begin, T* end, int step);
	T& operator*();
	T* operator->();
	CellIterator& operator++();
	CellIterator operator++(int);
	friend bool operator==(const CellIterator&, const CellIterator&) = default;

	int* begin()
	{
		return _it;
	}

	int* end()
	{
		return _end;
	}

private:
	T *_it, *_end;
	int _step;
};

template<Cell T>
class RowIterator {
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using reference = T&;

	RowIterator(T* begin, int step);
	T& operator*();
	T* operator->();
	RowIterator& operator++();
	RowIterator operator++(int);
	friend bool operator==(const RowIterator&, const RowIterator&) = default;

	[[nodiscard]] CellIterator<T> begin() { return _cell_begin; }
	[[nodiscard]] CellIterator<T> end() { return _cell_end; }
	[[nodiscard]] const CellIterator<T> begin() const { return _cell_begin; }
	[[nodiscard]] const CellIterator<T> end() const { return _cell_end; }
	[[nodiscard]] const CellIterator<T> cbegin() const { return _cell_begin; }
	[[nodiscard]] const CellIterator<T> cend() const { return _cell_end; }

private:
	T *_it, *_end;
	int _step;
	CellIterator<T> _cell_begin, _cell_end;
};

template<Cell T>
class GridRows {
public:
	GridRows(T* begin, T* end, int step) :
		_begin{begin, step},
		_end{end, step}
	{}

	[[nodiscard]] RowIterator<T> begin() { return _begin; }
	[[nodiscard]] RowIterator<T> end() { return _end; }
	[[nodiscard]] const RowIterator<T> begin() const { return _begin; }
	[[nodiscard]] const RowIterator<T> end() const { return _end; }
	[[nodiscard]] const RowIterator<T> cbegin() const { return _begin; }
	[[nodiscard]] const RowIterator<T> cend() const { return _end; }

private:
	RowIterator<T> _begin, _end;
};

template<Cell T>
class Grid {
public:
	Grid(int width, int height);
	Grid(const Grid&) = delete;
	Grid& operator=(const Grid&) = delete;
	Grid(Grid&&) noexcept;
	Grid& operator=(Grid&&) noexcept;
	~Grid();

	[[nodiscard]] int width() const { return _width; }
	[[nodiscard]] int height() const { return _height; }

	[[nodiscard]] GridRows<T> Rows();

private:
	T *_begin, *_end;
	int _width, _height;
	void _move(Grid&) noexcept;
	void _destroy() noexcept;
};

}

namespace shoujin {

template<Cell T> CellIterator<T>::CellIterator(T* begin, T* end, int step) :
	_it{begin},
	_end{end},
	_step{step}
{}

template<Cell T> T& CellIterator<T>::operator*() { return *_it; }

template<Cell T> T* CellIterator<T>::operator->() { return _it; }

template<Cell T> CellIterator<T>& CellIterator<T>::operator++()
{
	_it += _step;
	return *this;
}

template<Cell T> CellIterator<T> CellIterator<T>::operator++(int)
{
	auto self = *this;
	++*this;
	return self;
}

template<Cell T> RowIterator<T>::RowIterator(T* begin, int step) :
	_it{begin},
	_end{begin + step},
	_step{step},
	_cell_begin{_it, _end, step},
	_cell_end{_end, _end, step}
{}

template<Cell T> T& RowIterator<T>::operator*() { return *_it; }

template<Cell T> T* RowIterator<T>::operator->() { return _it; }

template<Cell T> RowIterator<T>& RowIterator<T>::operator++()
{
	_it += _step;
	return *this;
}

template<Cell T> RowIterator<T> RowIterator<T>::operator++(int)
{
	auto self = *this;
	++*this;
	return self;
}

template<Cell T> Grid<T>::Grid(int width, int height) :
	_width{width},
	_height{height}
{
	auto size = _width * _height;
	_begin = new T[size];
	_end = _begin + size;

	for(auto& it = _begin; it != _end; ++it)
		*it = {};
}

template<Cell T> Grid<T>::Grid(Grid&& rhs) noexcept { _move(rhs); }

template<Cell T> Grid<T>& Grid<T>::operator=(Grid&& rhs) noexcept
{
	if(this != &rhs) {
		_destroy();
		_move(rhs);
	}

	return *this;
}

template<Cell T> Grid<T>::~Grid() { _destroy(); }

template<Cell T> [[nodiscard]] GridRows<T> Grid<T>::Rows()
{
	return GridRows<T>{_begin, _end, _width};
}

template<Cell T> void Grid<T>::_move(Grid& rhs) noexcept
{
	_begin = rhs._begin;
	_end = rhs._end;
	_width = rhs._width;
	_height = rhs._height;
	rhs._begin = nullptr;
	rhs._end = nullptr;
	rhs._width = 0;
	rhs._height = 0;
}

template<Cell T> void Grid<T>::_destroy() noexcept
{
	if(_begin) {
		_begin = nullptr;
		_end = nullptr;
		_width = 0;
		_height = 0;
	}
}

}
