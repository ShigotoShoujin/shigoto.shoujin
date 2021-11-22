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

	CellIterator(T* begin, int step);
	T& operator*();
	T* operator->();
	CellIterator& operator++();
	CellIterator operator++(int);
	friend bool operator==(const CellIterator&, const CellIterator&) = default;

private:
	T* _it;
	int _step;
};

template<typename IteratorType>
class Iterable {
public:
	Iterable(const IteratorType& begin, const IteratorType& end) : _begin{begin}, _end{end} {}

private:
	IteratorType _begin, _end;

	[[nodiscard]] IteratorType* begin() { return _begin; }
	[[nodiscard]] IteratorType* end() { return _end; }
	[[nodiscard]] const IteratorType* begin() const { return _begin; }
	[[nodiscard]] const IteratorType* end() const { return _end; }
	[[nodiscard]] const IteratorType* cbegin() const { return _begin; }
	[[nodiscard]] const IteratorType* cend() const { return _end; }
};

template<Cell T>
class RowIterator : public CellIterator<T>, public Iterable<CellIterator<T>> {
public:
	RowIterator(T* begin, T* end, int step) :
		CellIterator{begin, step},
		Iterable{begin, end}
		{}
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

	[[nodiscard]] T* begin() { return _begin; }
	[[nodiscard]] T* end() { return _end; }
	[[nodiscard]] const T* begin() const { return _begin; }
	[[nodiscard]] const T* end() const { return _end; }
	[[nodiscard]] const T* cbegin() const { return _begin; }
	[[nodiscard]] const T* cend() const { return _end; }

	[[nodiscard]] Iterable<RowIterator<T>> Rows();

private:
	T *_begin, *_end;
	int _width, _height;
	void _move(Grid&) noexcept;
	void _destroy() noexcept;
};

}

namespace shoujin {

template<Cell T> CellIterator<T>::CellIterator(T* begin, int step) :
	_it{begin}, _step{step} {}

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

template<Cell T> Grid<T>::Grid(int width, int height) :
	_width{width},
	_height{height}
{
	auto size = width * height;
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

template<Cell T> [[nodiscard]] Iterable<RowIterator<T>> Grid<T>::Rows()
{
	return {_begin, _end, _width};
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
