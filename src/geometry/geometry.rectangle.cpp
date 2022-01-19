module;
#include <type_traits>

export module Shoujin.Geometry : Rectangle;
import : Point;
import : Size;

export namespace shoujin::geometry {

template<typename T>
requires requires { std::is_arithmetic_v<T>; }
struct Rectangle {
	T x1{};
	T y1{};
	T x2{};
	T y2{};

	constexpr Rectangle() = default;

	constexpr Rectangle(T const& x1, T const& y1, T const& x2, T const& y2) :
		x1{x1}, y1{y1}, x2{x2}, y2{y2} {}

	constexpr Rectangle(Point<T> const& position, Size<T> const& size) :
		x1{position.x}, y1{position.y}, x2{position.x + size.x}, y2{position.y + size.y} {}

	[[nodiscard]] constexpr int width() const { return x2 - x1; }
	[[nodiscard]] constexpr int height() const { return y2 - y1; }

	constexpr operator bool() const { return x1 || y1 || x2 || y2; }
	constexpr operator Size<T>() const { return RectangleToSize(*this); }
	constexpr friend bool operator==(Rectangle const& lhs, Rectangle const& rhs) { return lhs.x1 == rhs.x1 && lhs.y1 == rhs.y1 && lhs.x2 == rhs.x2 && lhs.y2 == rhs.y2; }
};

template<typename T>
requires requires { std::is_arithmetic_v<T>; }
constexpr inline Size<T> RectangleToSize(Rectangle<T> const& rect) { return {rect.x2 - rect.x1, rect.y2 - rect.y1}; }

using RectangleI = Rectangle<int>;
using RectangleF = Rectangle<float>;
using RectangleD = Rectangle<double>;

}
