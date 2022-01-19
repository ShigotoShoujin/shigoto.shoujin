module;
#include <type_traits>

export module Shoujin.Geometry : Vector2d;

export namespace shoujin::geometry {

template<typename T>
requires requires { std::is_arithmetic_v<T>; }
struct Vector2d {
	T x{};
	T y{};

	constexpr Vector2d() = default;

	constexpr Vector2d(T const& x, T const& y) :
		x{x}, y{y} {}

	constexpr operator bool() const noexcept { return x || y; }

	constexpr friend bool operator==(Vector2d const& lhs, Vector2d const& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }

	constexpr friend Vector2d operator+(Vector2d const& lhs, Vector2d const& rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y}; }
	constexpr friend Vector2d operator-(Vector2d const& lhs, Vector2d const& rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }

	constexpr friend Vector2d operator+(Vector2d const& lhs, int rhs) { return {lhs.x + rhs, lhs.y + rhs}; }
	constexpr friend Vector2d operator-(Vector2d const& lhs, int rhs) { return {lhs.x - rhs, lhs.y - rhs}; }
	constexpr friend Vector2d operator*(Vector2d const& lhs, int rhs) { return {lhs.x * rhs, lhs.y * rhs}; }
	constexpr friend Vector2d operator/(Vector2d const& lhs, int rhs) { return {lhs.x / rhs, lhs.y / rhs}; }
};

using Vector2dI = Vector2d<int>;
using Vector2dF = Vector2d<float>;
using Vector2dD = Vector2d<double>;

}
