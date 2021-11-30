#pragma once
#include "iterable.hpp"
#include "forward_step_iterator.hpp"

namespace shoujin {

template<typename T>
class RowIterator : public ForwardStepIterator<ForwardStepIterator<T>>, public Iterable<ForwardStepIterator<T>> {
public:
	RowIterator() {}

	RowIterator(T* begin, T* end, int step) :
		Iterable<ForwardStepIterator<T>>{{begin, step}, {end, step}},
		iterator{begin, step}
	{
		ForwardStepIterator<ForwardStepIterator<T>>::Reset(&iterator, 1);
	}

	ForwardStepIterator<T> begin()
	{
		_sync_range();
		return Iterable<ForwardStepIterator<T>>::begin();
	}

	ForwardStepIterator<T> end()
	{
		_sync_range();
		return Iterable<ForwardStepIterator<T>>::end();
	}

private:
	ForwardStepIterator<T> iterator;
	void _sync_range()
	{
		Iterable<ForwardStepIterator<T>>::Reset(ForwardStepIterator<ForwardStepIterator<T>>::operator*());
	}
};

}
