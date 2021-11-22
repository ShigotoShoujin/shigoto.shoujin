#pragma once
#include <iostream>
#include <iomanip>

struct RowIterator {
	int *_it, *_end;

	RowIterator(int* begin, int* end) :
		_it{begin},
		_end{end}
	{}

	int& operator*()
	{
		return *_it;
	}

	int* operator->()
	{
		return _it;
	}

	RowIterator& operator++()
	{
		++_it;
		return *this;
	}

	RowIterator operator++(int)
	{
		auto self = *this;
		++*this;
		return *this;
	}

	friend bool operator==(RowIterator lhs, RowIterator rhs)
	{
		return lhs._it == rhs._it;
	}

	friend bool operator!=(RowIterator lhs, RowIterator rhs)
	{
		return lhs._it != rhs._it;
	}

	int* begin()
	{
		return _it;
	}

	int* end()
	{
		return _end;
	}
};

struct RowsIterator {
	int *_it, *_end;
	int _row_length;
	RowIterator _row_begin, _row_end;

	RowsIterator(int* begin, int row_length) :
		_it{begin},
		_end{begin + row_length},
		_row_length{row_length},
		_row_begin{_it, _end},
		_row_end{_end, _end}
	{}

	RowIterator& operator*()
	{
		return _row_begin;
	}

	RowIterator* operator->()
	{
		return &_row_begin;
	}

	RowsIterator& operator++()
	{
		_it += _row_length;
		_end += _row_length;
		_row_begin = {_it, _end};
		_row_end = {_end, _end};
		return *this;
	}

	RowsIterator operator++(int)
	{
		auto self = *this;
		++*this;
		return *this;
	}

	friend bool operator==(RowsIterator lhs, RowsIterator rhs)
	{
		return lhs._it == rhs._it;
	}

	friend bool operator!=(RowsIterator lhs, RowsIterator rhs)
	{
		return lhs._it != rhs._it;
	}

	RowIterator begin() const
	{
		return _row_begin;
	}

	RowIterator end() const
	{
		return _row_end;
	}
};

struct GridRows {
	RowsIterator _begin, _end;

	GridRows(int* begin, int* end, int row_length) :
		_begin{begin, row_length},
		_end{end, row_length}
	{}

	RowsIterator begin()
	{
		return _begin;
	}

	RowsIterator end()
	{
		return _end;
	}
};

struct Grid {
	Grid(int width, int height) :
		_width{width}, _height{height}
	{
		size_t size = width * height;
		_begin = new int[size];
		_end = _begin + size;

		InitializeValues();
	}

	~Grid()
	{
		if(_begin) {
			delete[] _begin;
			_begin = nullptr;
			_end = nullptr;
		}
	}

	void InitializeValues()
	{
		for(int i = 0; auto&& it : *this)
			it = ++i;
	}

	int* begin() const { return _begin; }
	int* end() const { return _end; }

	GridRows Rows() const
	{
		return GridRows{_begin, _end, _width};
	}

	int *_begin, *_end;
	int _width, _height;
};

void IterateWithFor(const Grid& grid)
{
	std::cout << "Iterate though all rows cells\n";
	auto&& rows = grid.Rows();
	for(auto&& row_it = rows.begin(); row_it != rows.end(); ++row_it) {
		for(auto&& cell = row_it.begin(); cell != row_it.end(); ++cell) {
			std::cout << std::setw(2) << std::setfill('0') << *cell << ',';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void IterateWithRangeFor(const Grid& grid)
{
	std::cout << "Iterate though all rows cells\n";
	for(auto&& row_it : grid.Rows()) {
		for(auto&& cell : row_it)
			std::cout << std::setw(2) << std::setfill('0') << cell << ',';
		std::cout << '\n';
	}
	std::cout << '\n';
}
