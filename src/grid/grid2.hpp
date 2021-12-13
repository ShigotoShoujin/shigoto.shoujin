#pragma once
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <vector>

namespace shoujin {

class Row2 {
public:
	using value_type = int;
	using pointer = value_type*;
	using const_pointer = value_type const*;
	using reference = value_type&;
	using const_reference = value_type const&;
	using iterator = pointer;
	using const_iterator = const_pointer;

	constexpr Row2() = default;

	constexpr Row2(pointer begin, pointer end) :
		_begin{begin},
		_end{end} {}

	[[nodiscard]] constexpr iterator begin() noexcept { return _begin; }
	[[nodiscard]] constexpr iterator end() noexcept { return _end; };
	[[nodiscard]] constexpr const_iterator begin() const noexcept { return _begin; }
	[[nodiscard]] constexpr const_iterator end() const noexcept { return _end; };
	[[nodiscard]] constexpr const_iterator cbegin() const noexcept { return _begin; }
	[[nodiscard]] constexpr const_iterator cend() const noexcept { return _end; };

	[[nodiscard]] constexpr size_t size() const noexcept { return _end - _begin; }
	[[nodiscard]] constexpr reference operator[](size_t idx) noexcept { return *(_begin + idx); };
	[[nodiscard]] constexpr const_reference operator[](size_t idx) const noexcept { return *(_begin + idx); };

	[[nodiscard]] constexpr friend bool operator==(Row2 const& lhs, Row2 const& rhs)
	{
		return lhs._begin == rhs._begin && lhs._end == rhs._end;
	}

private:
	pointer _begin{};
	pointer _end{};
};

/// <summary>
/// ConstRowIterator is a LegacyRandomAccessIterator
/// https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator
/// </summary>
class ConstRowIterator2 {
public:
	using T = int;
	using value_type = Row2;
	using pointer = value_type const*;
	using reference = value_type const&;
	using difference_type = ptrdiff_t;
	using iterator_category = std::random_access_iterator_tag;

	constexpr ConstRowIterator2() = default;

	constexpr ConstRowIterator2(T* begin, difference_type row_length) :
		_begin{begin},
		_row{begin, begin + row_length},
		_row_length{row_length}
	{}

	constexpr void swap(ConstRowIterator2& right) noexcept
	{
		if(this != &right) {
			using std::swap;
			swap(_row, right._row);
			swap(_row_length, right._row_length);
		}
	}

	constexpr friend void swap(ConstRowIterator2& left, ConstRowIterator2& right) noexcept
	{
		left.swap(right);
	}

	[[nodiscard]] constexpr value_type operator[](difference_type idx) const noexcept { return {_begin, _begin + _row_length}; }
	[[nodiscard]] constexpr reference operator*() const noexcept { return _row; }
	[[nodiscard]] constexpr pointer operator->() const noexcept { return &_row; }

	constexpr ConstRowIterator2& operator++() noexcept
	{
		_row = {_row.begin() + _row_length, _row.end() + _row_length};
		return *this;
	}

	[[nodiscard]] constexpr ConstRowIterator2 operator++(int) noexcept
	{
		ConstRowIterator2 previous{*this};
		++*this;
		return previous;
	}

	constexpr ConstRowIterator2& operator--() noexcept
	{
		_row = {_row.begin() - _row_length, _row.end() - _row_length};
		return *this;
	}

	[[nodiscard]] constexpr ConstRowIterator2 operator--(int) noexcept
	{
		ConstRowIterator2 previous{*this};
		--*this;
		return previous;
	}

	[[nodiscard]] constexpr friend bool operator==(ConstRowIterator2 const& lhs, ConstRowIterator2 const& rhs)
	{
		return lhs._row == rhs._row;
	}

private:
	T* _begin{};
	Row2 _row;
	difference_type _row_length{};
};

/// <summary>
/// ConstRowIterator is a LegacyRandomAccessIterator
/// https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator
/// </summary>
class RowIterator2 : public ConstRowIterator2 {
public:
	using pointer = value_type*;
	using reference = value_type&;

	constexpr RowIterator2() = default;

	constexpr RowIterator2(int* begin, difference_type row_length) :
		ConstRowIterator2{begin, row_length} {}

	[[nodiscard]] constexpr reference operator*() const noexcept
	{
		return const_cast<reference>(ConstRowIterator2::operator*());
	}

	[[nodiscard]] constexpr pointer operator->() const noexcept
	{
		return const_cast<pointer>(ConstRowIterator2::operator->());
	}

	constexpr RowIterator2& operator++() noexcept
	{
		ConstRowIterator2::operator++();
		return *this;
	}

	[[nodiscard]] constexpr RowIterator2 operator++(int) noexcept
	{
		RowIterator2 previous{*this};
		++*this;
		return previous;
	}

	constexpr RowIterator2& operator--() noexcept
	{
		ConstRowIterator2::operator--();
		return *this;
	}

	[[nodiscard]] constexpr RowIterator2 operator--(int) noexcept
	{
		RowIterator2 previous{*this};
		--*this;
		return previous;
	}
};

//class Rows {
//public:
//	RowIterator2 begin
//};

/// <summary>
/// Grid2 is a Container
/// https://en.cppreference.com/w/cpp/named_req/Container
/// </summary>
class Grid2 {
public:
	using value_type = int;
	using reference = value_type&;
	using const_reference = value_type const&;
	using iterator = std::vector<int>::iterator;
	using const_iterator = std::vector<int>::const_iterator;
	using difference_type = std::vector<int>::difference_type;
	using size_type = std::vector<int>::size_type;

	constexpr Grid2() = default;

	constexpr Grid2(size_type width, size_type height) :
		_vector(width * height),
		_width{width},
		_height{height}
	{}

	constexpr Grid2(size_type width, size_type height, std::initializer_list<int> il) :
		_vector(width * height),
		_width{width},
		_height{height}
	{
		auto end = il.end() - (il.size() > _vector.size() ? il.size() - _vector.size() : 0);
		std::copy(il.begin(), end, _vector.begin());
	}

	[[nodiscard]] constexpr iterator begin() noexcept { return _vector.begin(); }
	[[nodiscard]] constexpr iterator end() noexcept { return _vector.end(); };
	[[nodiscard]] constexpr const_iterator begin() const noexcept { return _vector.begin(); }
	[[nodiscard]] constexpr const_iterator end() const noexcept { return _vector.end(); };
	[[nodiscard]] constexpr const_iterator cbegin() const noexcept { return _vector.cbegin(); }
	[[nodiscard]] constexpr const_iterator cend() const noexcept { return _vector.cend(); };
	[[nodiscard]] constexpr bool empty() const noexcept { return _vector.empty(); }
	[[nodiscard]] constexpr size_type size() const noexcept { return _vector.size(); }
	[[nodiscard]] constexpr size_type max_size() const noexcept { return _vector.max_size(); }

	[[nodiscard]] constexpr value_type* data() noexcept { return _vector.data(); }
	[[nodiscard]] constexpr value_type const* data() const noexcept { return const_cast<value_type const*>(_vector.data()); }
	[[nodiscard]] constexpr reference operator[](size_type idx) noexcept { return _vector[idx]; }
	[[nodiscard]] constexpr const_reference operator[](size_type idx) const noexcept { return _vector[idx]; }
	[[nodiscard]] constexpr size_type width() const noexcept { return _width; }
	[[nodiscard]] constexpr size_type height() const noexcept { return _height; }

	[[nodiscard]] constexpr friend bool operator==(Grid2 const& lhs, Grid2 const& rhs) noexcept
	{
		return lhs.width() == rhs.width() &&
			lhs.height() == rhs.height() &&
			std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	constexpr void swap(Grid2& right) noexcept
	{
		if(this != &right) {
			using std::swap;
			swap(_vector, right._vector);
			swap(_width, right._width);
			swap(_height, right._height);
		}
	}

	constexpr friend void swap(Grid2& left, Grid2& right) noexcept
	{
		left.swap(right);
	}

private:
	std::vector<int> _vector;
	size_type _width{};
	size_type _height{};
};

}
