module;
#include <type_traits>

export module Shoujin.Geometry;

export import : Point;
export import : Rectangle;
export import : Size;
export import : Vector2d;

export namespace shoujin::geometry {

template<typename T>
requires requires { std::is_arithmetic_v<T>; }
constexpr Point<T> clampPointToRect(Point<T> const& point, Rectangle<T> const& rect)
{
	auto pt = point;

	if(pt.x < rect.x1)
		pt.x = rect.x1;
	else if(pt.x >= rect.x2)
		pt.x = rect.x2 - 1;

	if(pt.y < rect.y1)
		pt.y = rect.y1;
	else if(pt.y >= rect.y2)
		pt.y = rect.y2 - 1;

	return pt;
}

}
