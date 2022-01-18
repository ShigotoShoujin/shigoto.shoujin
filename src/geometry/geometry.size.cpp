module;

export module Shoujin.Geometry : Size;
import : Vector2d;

export namespace shoujin::geometry {

template<typename T>
struct Size : public Vector2d<T> {
	constexpr Size() = default;

	constexpr Size(T const& x, T const& y) :
		Vector2d<T>{x, y} {}

	constexpr Size(Vector2d<T> const& rhs) :
		Vector2d<T>{rhs} {}
};

using SizeI = Size<int>;
using SizeF = Size<float>;
using SizeD = Size<double>;

}
