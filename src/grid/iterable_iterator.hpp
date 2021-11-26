//#pragma once
//#include "iterable.hpp"
//#include "forward_iterator.hpp"
//
//template<typename T>
//class IterableIterator : public ForwardIterator<T>, public Iterable<T>
//{
//public:
//	IterableIterator(T* iterator_begin, int iterator_step, T* iterable_begin, T* iterable_end) :
//		ForwardIterator<T>{ iterator_begin, iterator_step },
//		Iterable<T>{iterable_begin, iterable_begin}
//	{}
//
//	T* begin()
//	{
//		_sync_range();
//		return Iterable<T>::begin();
//	}
//
//	T* end()
//	{
//		_sync_range();
//		return Iterable<T>::end();
//	}
//
//private:
//	void _sync_range()
//	{
//		auto&& a = ForwardIterator<T>::operator->();
//		Iterable<T>::MoveRange(a);
//	}
//};
