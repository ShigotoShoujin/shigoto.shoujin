#include "bitmap_bits.hpp"

namespace shoujin::gui {

BitmapBits::BitmapBits(int width, int height) :
	_width{width}, _height{height}
{
	auto size = width * height;
	_pixels = std::make_unique<Pixel[]>(size);
	_end = _pixels.get() + size;
}

}
