#ifndef SHOUJIN_SOURCE_GUI_BITMAP_EFFECT_GRADIENT_BAR
#define SHOUJIN_SOURCE_GUI_BITMAP_EFFECT_GRADIENT_BAR

#include <cstdint>

namespace shoujin::gui::bitmap::effect {

class GradientBar {
	struct Increment {
		float R;
		float G;
		float B;
	};

	//Increment _map[6] = {
	//	{+0.f, +0.f, +1.f},
	//	{-1.f, +0.f, +0.f},
	//	{+0.f, +1.f, +0.f},
	//	{+0.f, +0.f, -1.f},
	//	{+1.f, +0.f, +0.f},
	//	{+0.f, -1.f, +0.f},
	//};

	struct Val {
		float R;
		float G;
		float B;
	};

public:
	struct Color {
		uint8_t R{};
		uint8_t G{};
		uint8_t B{};

		Color() = default;

		Color(Val const& val) :
			R{static_cast<uint8_t>(val.R)},
			G{static_cast<uint8_t>(val.G)},
			B{static_cast<uint8_t>(val.B)}
		{}
	};

	GradientBar(int total) :
		_total{total},
		_step{255 / (_total / 6.f)},
		_val{0xff, 0, 0},
		_ref{&_val.B}
	{}

	inline Color NextPixel()
	{
		auto result = _val;
		auto& r = *_ref;

		r += _step;

		if(r > 0xff || r < 0) {
			r = r < 0 ? 0.f : 255.f;
			switch(++_i) {
				case 1: _ref = &_val.R; break;
				case 2: _ref = &_val.G; break;
				case 3: _ref = &_val.B; break;
				case 4: _ref = &_val.R; break;
				case 5: _ref = &_val.G; break;
				default: return {};
			}
			_step = -_step;
		}

		return result;
	}

private:
	const float _kMax = 256 * 6;
	int _total;
	float _step;
	Val _val;
	float* _ref;
	int _i{0};
};

}

#endif
