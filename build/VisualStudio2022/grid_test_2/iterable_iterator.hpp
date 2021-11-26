#pragma once
#include "iterable.hpp"
#include "forward_iterator.hpp"

template<typename T>
class IterableIterator : public ForwardIterator<IteratorT>, public Iterable<IterableT>
{
public:
	IterableIterator(IteratorT iterator_begin, int iterator_step, IterableT iterable_begin, IterableT iterable_end) :
		ForwardIterator<IteratorT>{ &iterator_begin, iterator_step },
		Iterable<IterableT>{iterable_begin, iterable_begin}
	{}

	IterableT begin()
	{
		_sync_range();
		return Iterable<IterableT>::begin();
	}

	IterableT end()
	{
		_sync_range();
		return Iterable<IterableT>::end();
	}

private:
	void _sync_range()
	{
		auto&& a = ForwardIterator<IteratorT>::operator->();
		Iterable<IterableT>MoveRange();
	}
};
