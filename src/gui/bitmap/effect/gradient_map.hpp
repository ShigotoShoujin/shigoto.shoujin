#ifndef SHOUJIN_SOURCE_GUI_BITMAP_EFFECT_GRADIENT_MAP
#define SHOUJIN_SOURCE_GUI_BITMAP_EFFECT_GRADIENT_MAP

#include <cstdint>

namespace shoujin::gui::bitmap::effect {

class GradientMap {
	class Channel {
		struct ChannelParam {
			uint8_t TopLeft;
			uint8_t TopRight;
			uint8_t BottomLeft;
			uint8_t BottomRight;
		};

		ChannelParam _cp;
		int _width;
		float _vertical_step_left;
		float _vertical_step_right;
		float _line_start;
		float _line_end;
		float _line_step;
		float _value;

	public:
		Channel(ChannelParam cp, int width, int height) :
			_cp{cp},
			_width{width - 1},
			_line_step{},
			_value{}
		{
			auto h = static_cast<float>(height - 1);
			_vertical_step_left = (_cp.BottomLeft - _cp.TopLeft) / h;
			_vertical_step_right = (_cp.BottomRight - _cp.TopRight) / h;
			_line_start = _cp.TopLeft - _vertical_step_left;
			_line_end = _cp.TopRight - _vertical_step_right;
		}

		void NextRow()
		{
			_line_start += _vertical_step_left;
			_line_end += _vertical_step_right;
			_line_step = (_line_end - _line_start) / _width;
			_value = _line_start - _line_step;
		}

		inline uint8_t NextPixel()
		{
			return static_cast<uint8_t>(_value += _line_step);
		}
	};

	Channel _ch_R;
	Channel _ch_G;
	Channel _ch_B;

public:
	struct Color {
		uint8_t R;
		uint8_t G;
		uint8_t B;
	};

	struct GradientParam {
		Color TopLeft;
		Color TopRight;
		Color BottomLeft;
		Color BottomRight;
	};

	GradientMap(GradientParam gp, int width, int height) :
		_ch_R{{gp.TopLeft.R, gp.TopRight.R, gp.BottomLeft.R, gp.BottomRight.R}, width, height},
		_ch_G{{gp.TopLeft.G, gp.TopRight.G, gp.BottomLeft.G, gp.BottomRight.G}, width, height},
		_ch_B{{gp.TopLeft.B, gp.TopRight.B, gp.BottomLeft.B, gp.BottomRight.B}, width, height}
	{}

	void NextRow()
	{
		_ch_R.NextRow();
		_ch_G.NextRow();
		_ch_B.NextRow();
	}

	inline Color NextPixel()
	{
		return {
			_ch_R.NextPixel(),
			_ch_G.NextPixel(),
			_ch_B.NextPixel()};
	}
};

}

#endif
