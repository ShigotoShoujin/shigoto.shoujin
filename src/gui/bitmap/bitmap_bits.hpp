#ifndef SHOUJIN_SOURCE_GUI_BITMAP_BITMAP_BITS
#define SHOUJIN_SOURCE_GUI_BITMAP_BITMAP_BITS

#include "../../grid.hpp"
#include "../color.hpp"
#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui::bitmap {

struct Pixel {
	uint8_t b, g, r, a;
};

class BitmapBits : public Grid<Pixel> {
	using Grid = Grid<Pixel>;

public:
	BitmapBits(int width, int height) :
		Grid{width, height} {}

	[[nodiscard]] int width() const { return static_cast<int>(Grid::width()); }
	[[nodiscard]] int height() const { return static_cast<int>(Grid::height()); }

	void RenderGradientMap(Color const& top_left, Color const& top_right, Color const& bottom_left, Color const& bottom_right);
	void RenderHueBarHorizontal();
	void RenderHueBarVertical();
};

}

#endif
