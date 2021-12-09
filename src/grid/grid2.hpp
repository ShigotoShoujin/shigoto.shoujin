#pragma once
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

	constexpr Grid2(size_t width, size_t height) :
		_width{width},
		_height{height}
	{
		_vector.resize(width * height);
	}

	[[nodiscard]] constexpr iterator begin() noexcept { return _vector.begin(); }
	[[nodiscard]] constexpr iterator end() noexcept { return _vector.end(); };
	[[nodiscard]] constexpr const_iterator begin() const noexcept { return _vector.begin(); }
	[[nodiscard]] constexpr const_iterator end() const noexcept { return _vector.end(); };
	[[nodiscard]] constexpr const_iterator cbegin() const noexcept { return _vector.cbegin(); }
	[[nodiscard]] constexpr const_iterator cend() const noexcept { return _vector.cend(); };
	[[nodiscard]] constexpr std::vector<int>::pointer data() noexcept { return _vector.data(); }
	[[nodiscard]] constexpr size_t size() const noexcept { return _vector.size(); }
	[[nodiscard]] constexpr size_t width() const noexcept { return _width; }
	[[nodiscard]] constexpr size_t height() const noexcept { return _height; }

	[[nodiscard]] constexpr friend bool operator==(Grid2 const& lhs, Grid2 const& rhs)
	{
		if(lhs.width() != rhs.width() || lhs.height() != rhs.height())
			return false;

		auto&& a = lhs.cbegin();
		auto&& b = rhs.cbegin();
		auto&& end = lhs.cend();
		while(a < end) {
			if(*a != *b)
				return false;
			++a;
			++b;
		}

		return true;
	}

	[[nodiscard]] constexpr friend bool operator!=(Grid2 const& lhs, Grid2 const& rhs)
	{
		return !(lhs == rhs);
	}

private:
	std::vector<int> _vector;
	size_t _width{};
	size_t _height{};
};

}
