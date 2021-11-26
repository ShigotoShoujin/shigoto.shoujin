#pragma once
#include "iterable.hpp"
#include "iterable_iterator.hpp"

template<typename T>
class GridRows : public Iterable<IterableIterator<ForwardIterator<T>>>
{
public:
	GridRows(T* begin, T* end, int width)
	{
		auto it_begin = ForwardIterator<T>{ begin, width };
		auto it_end = ForwardIterator<T>{ end, width };
		_begin = IterableIterator<ForwardIterator<T>>{ &it_begin, &it_end, width };
		_end = IterableIterator<ForwardIterator<T>>{ &it_end, &it_end, width };
		Iterable<IterableIterator<ForwardIterator<T>>>::_begin = _begin;
		Iterable<IterableIterator<ForwardIterator<T>>>::_end = _end;
	}

private:
	IterableIterator<ForwardIterator<T>> _begin;
	IterableIterator<ForwardIterator<T>> _end;
};
