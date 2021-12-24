#include "bitmap_bits.hpp"
#include "effect/gradient.hpp"

namespace shoujin::gui::bitmap {

using namespace effect;

void BitmapBits::RenderGradient(Color const& top_left, Color const& top_right, Color const& bottom_left, Color const& bottom_right)
{
	auto cast = [](Color const& c) -> Gradient::Color {
		COLORREF cr = c;
		return {GetRValue(cr), GetGValue(cr), GetBValue(cr)};
	};

	auto tl = cast(top_left);
	auto tr = cast(top_right);
	auto bl = cast(bottom_left);
	auto br = cast(bottom_right);

	Gradient gradient{{tl, tr, bl, br}, width(), height()};

	for(auto&& row : EnumerateRows()) {
		gradient.NextRow();
		for(auto&& pixel : row) {
			auto c = gradient.NextPixel();
			pixel.r = c.R;
			pixel.g = c.G;
			pixel.b = c.B;
		}
	}
}

}
