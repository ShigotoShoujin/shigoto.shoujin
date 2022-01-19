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

	constexpr Point<T>& ClampPoint(Vector2d<T> const& size)
	{
		using V = Vector2d<T>;

		if(V::x < 0)
			V::x = 0;
		else if(V::x >= size.x)
			V::x = size.x - 1;

		if(V::y < 0)
			V::y = 0;
		else if(V::y >= size.y)
			V::y = size.y - 1;

		return *this;
	}
};

using PointI = Point<int>;
using PointF = Point<float>;
using PointD = Point<double>;

}
