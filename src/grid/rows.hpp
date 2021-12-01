#pragma once
#include "iterable.hpp"
#include "iterator.hpp"

namespace shoujin {

template<typename T>
class RowIterator : public Iterable<T*> {
public:
	RowIterator(T* begin, T* end) :
		Iterable<T*>{begin, end}
	{}

	void NextRow()
	{
		size_t size = Iterable<T*>::end() - Iterable<T*>::begin();
		Iterable<T*>::Reset(Iterable<T*>::begin() + size, Iterable<T*>::end() + size);
	}
};

template<typename T>
class RowsIterator : public Iterator<RowIterator<T>> {
public:
	RowsIterator() {}

	RowsIterator(RowIterator<T>* begin, int step) :
		Iterator<RowIterator<T>>{begin, step}
	{}

	Iterator<RowIterator<T>>& operator++()
	{
		Iterator<RowIterator<T>>::operator->()->NextRow();
		Iterator<RowIterator<T>>::operator++();
		return *this;
	}

	Iterator<RowIterator<T>> operator++(int)
	{
		auto self = *this;
		++*this;
		return self;
	}
};

template<typename T>
class Rows : public Iterable<RowsIterator<T>> {
public:
	Rows(T* begin, T* end, int step) :
		_cell_begin{begin, begin + step},
		_cell_end{end, end},
		_row_begin{&_cell_begin, step},
		_row_end{&_cell_end, step}
	{
		Iterable<RowsIterator<T>>::Reset(_row_begin, _row_end);
	}

	RowsIterator<T> begin()
	{
		return Iterable<RowsIterator<T>>::begin();
	}

	RowsIterator<T> end()
	{
		return Iterable<RowsIterator<T>>::end();
	}

private:
	RowIterator<T> _cell_begin, _cell_end;
	RowsIterator<T> _row_begin, _row_end;
};

}
