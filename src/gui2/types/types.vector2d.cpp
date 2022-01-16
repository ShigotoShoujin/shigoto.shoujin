module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.Gui.Types : Vector2d;

export namespace shoujin::gui2 {

struct Vector2d {
	int x;
	int y;

	constexpr Vector2d() :
		x{}, y{} {}

	constexpr Vector2d(int x, int y) :
		x{x}, y{y} {}

	constexpr Vector2d(POINT const& rhs) :
		x{rhs.x}, y{rhs.y} {}

	constexpr Vector2d(SIZE const& rhs) :
		x{rhs.cx}, y{rhs.cy} {}

	constexpr operator bool() const { return x || y; }
	constexpr operator POINT() const { return {x, y}; }
	constexpr operator SIZE() const { return {x, y}; }

	constexpr friend bool operator==(Vector2d const& lhs, Vector2d const& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
	constexpr friend bool operator!=(Vector2d const& lhs, Vector2d const& rhs) { return !(lhs == rhs); }

	constexpr friend Vector2d operator+(Vector2d const& lhs, Vector2d const& rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y}; }
	constexpr friend Vector2d operator-(Vector2d const& lhs, Vector2d const& rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }

	constexpr friend Vector2d operator+(Vector2d const& lhs, int rhs) { return {lhs.x + rhs, lhs.y + rhs}; }
	constexpr friend Vector2d operator-(Vector2d const& lhs, int rhs) { return {lhs.x - rhs, lhs.y - rhs}; }
	constexpr friend Vector2d operator*(Vector2d const& lhs, int rhs) { return {lhs.x * rhs, lhs.y * rhs}; }
	constexpr friend Vector2d operator/(Vector2d const& lhs, int rhs) { return {lhs.x / rhs, lhs.y / rhs}; }
};

}
