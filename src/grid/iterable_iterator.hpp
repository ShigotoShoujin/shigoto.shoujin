#pragma once
#include "iterable.hpp"
#include "forward_step_iterator.hpp"

namespace shoujin {

template<typename T>
class IterableIterator : public ForwardStepIterator<T>, public Iterable<T> {
public:
	IterableIterator(T* begin, T* end, int step) :
		ForwardStepIterator<T>{begin, step},
		Iterable<T>{begin, end}
	{}

	T* begin()
	{
		_sync_range();
		return Iterable<T>::begin();
	}

	T* end()
	{
		_sync_range();
		return Iterable<T>::end();
	}

private:
	void _sync_range()
	{
		Iterable<T>::SetBegin(ForwardStepIterator<T>::operator->());
	}
};

}
