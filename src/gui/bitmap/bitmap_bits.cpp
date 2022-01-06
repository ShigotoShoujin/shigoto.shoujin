#include "bitmap_bits.hpp"
#include "effect/gradient_bar.hpp"
#include "effect/gradient_map.hpp"

namespace shoujin::gui::bitmap {

using namespace effect;

void BitmapBits::RenderGradientMap(Color const& top_left, Color const& top_right, Color const& bottom_left, Color const& bottom_right)
{
	auto cast = [](Color const& c) -> GradientMap::Color {
		COLORREF cr = c;
		return {GetRValue(cr), GetGValue(cr), GetBValue(cr)};
	};

	auto tl = cast(top_left);
	auto tr = cast(top_right);
	auto bl = cast(bottom_left);
	auto br = cast(bottom_right);

	GradientMap gm{{tl, tr, bl, br}, width(), height()};

	for(auto&& row : EnumerateRows()) {
		gm.NextRow();
		for(auto&& pixel : row) {
			auto c = gm.NextPixel();
			pixel.r = c.R;
			pixel.g = c.G;
			pixel.b = c.B;
		}
	}
}

void BitmapBits::RenderHueBarHorizontal()
{
	for(auto&& row : EnumerateRows()) {
		GradientBar gb{width()};
		for(auto&& pixel : row) {
			auto c = gb.NextPixel();
			pixel.r = c.R;
			pixel.g = c.G;
			pixel.b = c.B;
		}
	}
}

void BitmapBits::RenderHueBarVertical()
{
	GradientBar gb{height()};

	for(auto&& row : EnumerateRows()) {
		auto c = gb.NextPixel();
		for(auto&& pixel : row) {
			pixel.r = c.R;
			pixel.g = c.G;
			pixel.b = c.B;
		}
	}
}

}
