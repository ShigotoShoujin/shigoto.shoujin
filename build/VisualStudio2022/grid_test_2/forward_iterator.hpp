#pragma once
#include <iterator>

template<typename T>
class ForwardIterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = value_type*;
	using reference = value_type&;

	ForwardIterator(pointer begin, int step = 1) : _it{ begin }, _step{ step } {}

	reference operator*()
	{
		return *_it;
	}

	pointer operator->()
	{
		return _it;
	}

	ForwardIterator& operator++()
	{
		_it += _step;
		return *this;
	}

	ForwardIterator operator++(int)
	{
		auto self = *this;
		++* this;
		return self;
	}

	friend bool operator==(const ForwardIterator&, const ForwardIterator&) = default;

private:
	pointer _it;
	int _step;
};
