#pragma once
#include "iterable.hpp"
#include "iterator.hpp"

namespace shoujin {

template<typename T>
class RowIterator {
public:
	RowIterator() {}

	RowIterator(T* begin, T* end)
	{}

	T* begin() { return _begin; }
	T* end() { return _end; }

	void Reset(T* begin, T* end)
	{
		_begin = begin;
		_end = end;
	}

private:
	T *_begin, *_end;
};

template<typename T>
class RowsIterator {
public:
	RowsIterator() {}

	RowsIterator(T* begin, T* end, int step) :
		_it{begin},
		_begin{begin},
		_end{end},
		_step{step}
	{}

	[[nodiscard]] RowIterator<T>& operator*()
	{
		_row_iterator.Reset(_it, _it + _step);
		return _row_iterator;
	}

	[[nodiscard]] RowIterator<T>* operator->()
	{
		return &_row_iterator;
	}

	RowsIterator& operator++()
	{
		_it += _step;
		return *this;
	}

	RowsIterator operator++(int)
	{
		auto self = *this;
		++*this;
		return self;
	}

	[[nodiscard]] friend bool operator==(const RowsIterator<T>& lhs, const RowsIterator<T>& rhs)
	{
		return lhs._it == rhs._it;
	}

	[[nodiscard]] friend bool operator!=(const RowsIterator<T>& lhs, const RowsIterator<T>& rhs)
	{
		return lhs._it != rhs._it;
	}

private:
	T *_begin, *_end;
	T* _it;
	int _step;
	RowIterator<T> _row_iterator;
};

template<typename T>
class Rows : public Iterable<RowsIterator<T>> {
public:
	Rows(T* begin, T* end, int step) :
		Iterable<RowsIterator<T>>{{begin, end, step}, {end, end, step}}
	{}
};

}
