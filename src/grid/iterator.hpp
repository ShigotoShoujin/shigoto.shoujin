#pragma once
#include <iterator>

namespace shoujin {

template<typename T>
class Iterator {
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = value_type*;
	using reference = value_type&;

	Iterator() :
		_it{},
		_step{}
	{}

	Iterator(pointer begin, int step = 1) :
		_it{begin},
		_step{step}
	{}

	void Reset(pointer begin, int step = 1)
	{
		_it = begin;
		_step = step;
	}

	[[nodiscard]] reference operator*() { return *_it; }

	[[nodiscard]] pointer operator->() { return _it; }

	Iterator& operator++()
	{
		_it += _step;
		return *this;
	}

	Iterator operator++(int)
	{
		auto self = *this;
		++*this;
		return self;
	}

	//[[nodiscard]] friend bool operator==(const Iterator&, const Iterator&) = default;

	[[nodiscard]] friend bool operator==(const Iterator& lhs, const Iterator& rhs)
	{
		return lhs._it == rhs._it;
	}

	[[nodiscard]] friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
	{
		return lhs._it != rhs._it;
	}

private:
	pointer _it;
	int _step;
};

}
