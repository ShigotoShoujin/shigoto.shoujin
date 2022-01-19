module;

export module Shoujin.Geometry : Point;
import : Vector2d;

export namespace shoujin::geometry {

template<typename T>
struct Point : public Vector2d<T> {
	constexpr Point() = default;

	constexpr Point(T const& x, T const& y) :
		Vector2d<T>{x, y} {}

	constexpr Point(Vector2d<T> const& rhs) :
		Vector2d<T>{rhs} {}
};

using PointI = Point<int>;
using PointF = Point<float>;
using PointD = Point<double>;

}
