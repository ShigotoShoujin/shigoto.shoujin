#ifndef SHOUJIN_SOURCE_GRID
#define SHOUJIN_SOURCE_GRID

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <vector>

namespace shoujin {

/// <summary>
/// Grid is a Container
/// https://en.cppreference.com/w/cpp/named_req/Container
/// </summary>
template<typename T>
class Grid {
	//using T = int;

public:
	class Row {
	public:
		using value_type = T;
		using pointer = value_type*;
		using const_pointer = value_type const*;
		using reference = value_type&;
		using const_reference = value_type const&;
		using iterator = pointer;
		using const_iterator = const_pointer;

		constexpr Row() = default;

		constexpr Row(pointer begin, pointer end) :
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

		[[nodiscard]] constexpr friend bool operator==(Row const& lhs, Row const& rhs)
		{
			return lhs._begin == rhs._begin && lhs._end == rhs._end;
		}

	private:
		pointer _begin{};
		pointer _end{};
	};

	/// <summary>
	/// ConstRowIterator is a LegacyBidirectionalIterator
	/// https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator
	/// </summary>
	class ConstRowIterator {
	public:
		using value_type = Row;
		using pointer = value_type const*;
		using reference = value_type const&;
		using difference_type = ptrdiff_t;
		using iterator_category = std::random_access_iterator_tag;

		constexpr ConstRowIterator() = default;

		constexpr ConstRowIterator(T* begin, size_t row_length) :
			_begin{begin},
			_row{begin, begin + row_length},
			_row_length{row_length}
		{}

		constexpr void swap(ConstRowIterator& right) noexcept
		{
			if(this != &right) {
				using std::swap;
				swap(_row, right._row);
				swap(_row_length, right._row_length);
			}
		}

		constexpr friend void swap(ConstRowIterator& left, ConstRowIterator& right) noexcept
		{
			left.swap(right);
		}

		[[nodiscard]] constexpr value_type operator[](size_t idx) const noexcept { return {_begin, _begin + _row_length}; }
		[[nodiscard]] constexpr reference operator*() const noexcept { return _row; }
		[[nodiscard]] constexpr pointer operator->() const noexcept { return &_row; }

		constexpr ConstRowIterator& operator++() noexcept
		{
			_row = {_row.begin() + _row_length, _row.end() + _row_length};
			return *this;
		}

		[[nodiscard]] constexpr ConstRowIterator operator++(int) noexcept
		{
			ConstRowIterator previous{*this};
			++*this;
			return previous;
		}

		constexpr ConstRowIterator& operator--() noexcept
		{
			_row = {_row.begin() - _row_length, _row.end() - _row_length};
			return *this;
		}

		[[nodiscard]] constexpr ConstRowIterator operator--(int) noexcept
		{
			ConstRowIterator previous{*this};
			--*this;
			return previous;
		}

		[[nodiscard]] constexpr friend bool operator==(ConstRowIterator const& lhs, ConstRowIterator const& rhs)
		{
			return lhs._row == rhs._row;
		}

	private:
		T* _begin{};
		Row _row;
		size_t _row_length{};
	};

	/// <summary>
	/// ConstRowIterator is a LegacyBidirectionalIterator
	/// https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator
	/// </summary>
	class RowIterator : public ConstRowIterator {
	public:
		using pointer = ConstRowIterator::value_type*;
		using reference = ConstRowIterator::value_type&;

		constexpr RowIterator() = default;

		constexpr RowIterator(T* begin, size_t row_length) :
			ConstRowIterator{begin, row_length} {}

		[[nodiscard]] constexpr reference operator*() const noexcept
		{
			return const_cast<reference>(ConstRowIterator::operator*());
		}

		[[nodiscard]] constexpr pointer operator->() const noexcept
		{
			return const_cast<pointer>(ConstRowIterator::operator->());
		}

		constexpr RowIterator& operator++() noexcept
		{
			ConstRowIterator::operator++();
			return *this;
		}

		[[nodiscard]] constexpr RowIterator operator++(int) noexcept
		{
			RowIterator previous{*this};
			++*this;
			return previous;
		}

		constexpr RowIterator& operator--() noexcept
		{
			ConstRowIterator::operator--();
			return *this;
		}

		[[nodiscard]] constexpr RowIterator operator--(int) noexcept
		{
			RowIterator previous{*this};
			--*this;
			return previous;
		}
	};

	class ConstRows {
	public:
		constexpr ConstRows() {}

		constexpr ConstRows(T const* begin, T const* end, size_t row_length) :
			_begin{const_cast<T*>(begin)},
			_end{const_cast<T*>(end)},
			_row_length{row_length}
		{}

		[[nodiscard]] constexpr ConstRowIterator begin() const noexcept { return {_begin, _row_length}; }
		[[nodiscard]] constexpr ConstRowIterator end() const noexcept { return {_end, _row_length}; }
		[[nodiscard]] constexpr ConstRowIterator cbegin() const noexcept { return {_begin, _row_length}; }
		[[nodiscard]] constexpr ConstRowIterator cend() const noexcept { return {_end, _row_length}; }

	protected:
		T* _begin{};
		T* _end{};
		size_t _row_length{};
	};

	class Rows : public ConstRows {
	public:
		constexpr Rows() {}

		constexpr Rows(T* begin, T* end, size_t row_length) :
			ConstRows(begin, end, row_length) {}

		[[nodiscard]] constexpr RowIterator begin() noexcept { return {ConstRows::_begin, ConstRows::_row_length}; }
		[[nodiscard]] constexpr RowIterator end() noexcept { return {ConstRows::_end, ConstRows::_row_length}; }
	};

	using value_type = T;
	using reference = value_type&;
	using const_reference = value_type const&;
	using iterator = std::vector<T>::iterator;
	using const_iterator = std::vector<T>::const_iterator;
	using difference_type = std::vector<T>::difference_type;
	using size_type = std::vector<T>::size_type;

	constexpr Grid() = default;

	constexpr Grid(size_type width, size_type height) :
		_vector(width * height),
		_width{width},
		_height{height}
	{}

	constexpr Grid(size_type width, size_type height, std::initializer_list<T> il) :
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

	[[nodiscard]] constexpr friend bool operator==(Grid const& lhs, Grid const& rhs) noexcept
	{
		return lhs.width() == rhs.width() &&
			lhs.height() == rhs.height() &&
			std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	constexpr void swap(Grid& right) noexcept
	{
		if(this != &right) {
			using std::swap;
			swap(_vector, right._vector);
			swap(_width, right._width);
			swap(_height, right._height);
		}
	}

	constexpr friend void swap(Grid& left, Grid& right) noexcept
	{
		left.swap(right);
	}

	[[nodiscard]] constexpr Rows EnumerateRows() noexcept
	{
		return {data(), data() + size(), _width};
	}

	[[nodiscard]] constexpr ConstRows EnumerateRows() const noexcept
	{
		return {data(), data() + size(), _width};
	}

private:
	std::vector<T> _vector;
	size_type _width{};
	size_type _height{};
};

}

#endif
