#pragma once
#include <iterator>

namespace shoujin {

template<typename T>
class ForwardStepIterator {
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = value_type*;
	using reference = value_type&;

	ForwardStepIterator() :
		_it{},
		_step{}
	{}

	ForwardStepIterator(pointer begin, int step = 1) :
		_it{begin},
		_step{step}
	{}

	[[nodiscard]] reference operator*() { return *_it; }

	[[nodiscard]] pointer operator->() { return _it; }

	ForwardStepIterator& operator++()
	{
		_it += _step;
		return *this;
	}

	ForwardStepIterator operator++(int)
	{
		auto self = *this;
		++*this;
		return self;
	}

	//[[nodiscard]] friend bool operator==(const ForwardStepIterator&, const ForwardStepIterator&) = default;
	
	[[nodiscard]] friend bool operator==(const ForwardStepIterator& lhs, const ForwardStepIterator& rhs)
	{
		return lhs._it == rhs._it;
	}
	
	[[nodiscard]] friend bool operator!=(const ForwardStepIterator& lhs, const ForwardStepIterator& rhs)
	{
		return lhs._it != rhs._it;
	}

private:
	pointer _it;
	int _step;
};

}
