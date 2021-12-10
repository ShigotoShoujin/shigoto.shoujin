#pragma once
#include <algorithm>
#include <initializer_list>
#include <vector>

namespace shoujin {

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
	[[nodiscard]] constexpr value_type* data() noexcept { return _vector.data(); }
	[[nodiscard]] constexpr reference operator[](size_type idx) noexcept { return _vector[idx]; }
	[[nodiscard]] constexpr const_reference operator[](size_type idx) const noexcept { return _vector[idx]; }
	[[nodiscard]] constexpr size_type size() const noexcept { return _vector.size(); }
	[[nodiscard]] constexpr size_type width() const noexcept { return _width; }
	[[nodiscard]] constexpr size_type height() const noexcept { return _height; }

	[[nodiscard]] constexpr friend bool operator==(Grid2 const& lhs, Grid2 const& rhs) noexcept
	{
		return lhs.width() == rhs.width() &&
			lhs.height() == rhs.height() &&
			std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	[[nodiscard]] constexpr friend bool operator!=(Grid2 const& lhs, Grid2 const& rhs) noexcept
	{
		return !(lhs == rhs);
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

private:
	std::vector<int> _vector;
	size_type _width{};
	size_type _height{};
};

constexpr void swap(Grid2& left, Grid2& right) noexcept
{
	left.swap(right);
}

}
