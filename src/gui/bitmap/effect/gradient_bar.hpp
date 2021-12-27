#ifndef SHOUJIN_SOURCE_GUI_BITMAP_EFFECT_GRADIENT_BAR
#define SHOUJIN_SOURCE_GUI_BITMAP_EFFECT_GRADIENT_BAR

#include <cmath>
#include <cstdint>

namespace shoujin::gui::bitmap::effect {

class GradientBar {
	struct ColorF {
		float R;
		float G;
		float B;
	};

public:
	struct Color {
		uint8_t R{};
		uint8_t G{};
		uint8_t B{};
	};

	const int kMinSize = 7;

	GradientBar(int size) :
		_size{size},
		_step{0xff / ((_size - 1) / 6.f)},
		_color{0xff, 0, 0},
		_colorf{0xff, 0, 0},
		_c_ptr{&_color.B},
		_cf_ptr{&_colorf.B},
		_phase{}
	{}

	Color NextPixel()
	{
		if(_size < kMinSize)
			return {};

		if(*_cf_ptr > 0xff || *_cf_ptr < 0) {
			*_cf_ptr = *_cf_ptr < 0 ? 0.f : 255.f;
			switch(++_phase) {
				case 1:
					_cf_ptr = &_colorf.R;
					_c_ptr = &_color.R;
					break;
				case 2:
					_cf_ptr = &_colorf.G;
					_c_ptr = &_color.G;
					break;
				case 3:
					_cf_ptr = &_colorf.B;
					_c_ptr = &_color.B;
					break;
				case 4:
					_cf_ptr = &_colorf.R;
					_c_ptr = &_color.R;
					break;
				case 5:
					_cf_ptr = &_colorf.G;
					_c_ptr = &_color.G;
					break;
				default: return {};
			}
			_step = -_step;
			*_cf_ptr += _step;
		}

		*_c_ptr = static_cast<uint8_t>(std::roundf(*_cf_ptr));
		*_cf_ptr += _step;

		return _color;
	}

private:
	int _size;
	float _step;
	Color _color;
	ColorF _colorf;
	uint8_t* _c_ptr;
	float* _cf_ptr;
	int _phase;
};

}

#endif
