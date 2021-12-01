#pragma once
#include "iterable.hpp"
#include "iterator.hpp"

namespace shoujin {

template<typename T>
class Rows : public Iterable<Iterator<Iterable<T*>>> {
public:
	Rows(T* begin, T* end, int step) :
		_cell_begin{begin, begin + step},
		_cell_end{end, end},
		_row_begin{&_cell_begin, step},
		_row_end{&_cell_end, step}
	{
		Iterable<Iterator<Iterable<T*>>>::Reset(_row_begin, _row_end);
	}

	Iterator<Iterable<T*>> begin()
	{
		_sync_range();
		return Iterable<Iterator<Iterable<T*>>>::begin();
	}

	Iterator<Iterable<T*>> end()
	{
		_sync_range();
		return Iterable<Iterator<Iterable<T*>>>::end();
	}

private:
	Iterable<T*> _cell_begin, _cell_end;
	Iterator<Iterable<T*>> _row_begin, _row_end;

	void _sync_range()
	{
		//Iterable<Iterator<T>>::Reset(Iterator<Iterator<T>>::operator*());
	}
};

}
