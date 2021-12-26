#ifndef SHOUJIN_SOURCE_GUI_BITMAP_EFFECT_GRADIENT_BAR
#define SHOUJIN_SOURCE_GUI_BITMAP_EFFECT_GRADIENT_BAR

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

	GradientBar(int total) :
		_total{total},
		_step{0xff / (_total / 6.f)},
		_color{0xff, 0, 0},
		_colorf{0xff, 0, 0},
		_c_ptr{&_color.B},
		_cf_ptr{&_colorf.B},
		_phase{}
	{}

	inline Color NextPixel()
	{
		auto result = _color;
		auto& cf = *_cf_ptr;
		auto& c = *_c_ptr;

		cf += _step;

		if(cf > 0xff || cf < 0) {
			cf = cf < 0 ? 0.f : 255.f;
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
		}

		c = static_cast<uint8_t>(cf);

		return result;
	}

private:
	int _total;
	float _step;
	Color _color;
	ColorF _colorf;
	uint8_t* _c_ptr;
	float* _cf_ptr;
	int _phase;
};

}

#endif
