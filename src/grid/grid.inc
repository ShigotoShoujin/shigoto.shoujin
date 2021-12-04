template<typename T>
class RowIterator : public Iterable<T*> {
public:
	RowIterator() {}

	RowIterator(T* begin, T* end) :
		Iterable<T*>{begin, end}
	{}
};

template<typename T>
class RowsIterator {
public:
	RowsIterator() {}

	RowsIterator(T* begin, int step) :
		_it{begin},
		_step{step}
	{}

	[[nodiscard]] RowIterator<T>& operator*()
	{
		_row_iterator.Reset(_it, _it + _step);
		return _row_iterator;
	}

	[[nodiscard]] RowIterator<T>* operator->()
	{
		_row_iterator.Reset(_it, _it + _step);
		return &_row_iterator;
	}

	RowsIterator& operator++()
	{
		_it += _step;
		return *this;
	}

	RowsIterator operator++(int)
	{
		auto self = *this;
		++*this;
		return self;
	}

	[[nodiscard]] friend bool operator==(const RowsIterator& lhs, const RowsIterator& rhs)
	{
		return lhs._it == rhs._it;
	}

	[[nodiscard]] friend bool operator!=(const RowsIterator& lhs, const RowsIterator& rhs)
	{
		return lhs._it != rhs._it;
	}

private:
	T* _it;
	int _step;
	RowIterator<T> _row_iterator;
};
