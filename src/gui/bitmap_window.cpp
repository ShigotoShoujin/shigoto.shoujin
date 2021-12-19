#include "bitmap_window.hpp"

using namespace shoujin::gui;

namespace shoujin::gui {

BitmapWindow::BitmapWindow(LayoutParam const& lp) :
	Window{lp} {}

BitmapWindow::BitmapWindow(BitmapWindow const& rhs)
{
	_bitmap = std::make_unique<Bitmap>(*rhs._bitmap);
}

BitmapWindow& BitmapWindow::operator=(BitmapWindow const& rhs)
{
	if(this != &rhs)
		_bitmap = std::make_unique<Bitmap>(*rhs._bitmap);
	return *this;
}

Window::CreateParam BitmapWindow::OnCreateParam()
{
	return CreateParam{.classname = TEXT("ShoujinBitmapWindow")};
}

bool BitmapWindow::OnCreate(CREATESTRUCT const& createparam)
{
	_bitmap = std::make_unique<Bitmap>(client_size());

	struct colormap {
		struct channel {
			uint8_t _target;
			int _width;
			float _vertical_step_left;
			float _vertical_step_right;
			float _line_start;
			float _line_end;
			float _line_step;
			float _value;

			channel(uint8_t target, int width, int height) :
				_target{target},
				_width{width}
			{
				_line_start = 255.f;
				_line_end = _target;
				_vertical_step_left = _line_start / (height - 1);
				_vertical_step_right = _line_end / (height - 1);
				_line_step = (_line_start - _line_end) / (_width - 1);
				_value = _line_start + _line_step;
			}

			inline uint8_t next_pixel()
			{
				return static_cast<uint8_t>(_value -= _line_step);
			}

			void next_line()
			{
				_line_start -= _vertical_step_left;
				_line_end -= _vertical_step_right;
				_line_step = (_line_start - _line_end) / (_width - 1);
				_value = _line_start;
			}
		} r, g, b;

		struct color {
			uint8_t r;
			uint8_t g;
			uint8_t b;
		};

		colormap(int width, int height, color target_color) :
			r{target_color.r, width, height},
			g{target_color.g, width, height},
			b{target_color.b, width, height}
		{}

		inline color next_pixel()
		{
			return {
				r.next_pixel(),
				g.next_pixel(),
				b.next_pixel()};
		}

		void next_line()
		{
			r.next_line();
			g.next_line();
			b.next_line();
		}
	};

	auto bits = _bitmap->GetBits();

	colormap cm{bits.width(), bits.height(), {255, 112, 0}};

	for(auto&& row : bits.EnumerateRows()) {
		for(auto&& pixel : row) {
			auto c = cm.next_pixel();
			pixel.r = c.r;
			pixel.g = c.g;
			pixel.b = c.b;
		}
		cm.next_line();
	}

	_bitmap->SetBits(bits);

	return true;
}

bool BitmapWindow::OnPaint()
{
	HWND hwnd = handle()->hwnd();
	HDC source_hdc = _bitmap->hdc();

	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	int x = ps.rcPaint.left;
	int y = ps.rcPaint.top;
	int w = ps.rcPaint.right - x;
	int h = ps.rcPaint.bottom - y;
	BitBlt(ps.hdc, x, y, w, h, source_hdc, x, y, SRCCOPY);
	EndPaint(hwnd, &ps);

	return true;
}

bool BitmapWindow::OnSizingFinished()
{
	auto old_size = _bitmap->size();
	auto new_size = client_size();

	if(old_size != new_size) {
		_bitmap = std::make_unique<Bitmap>(client_size());
		_bitmap->Fill(Color::Lime);
		InvalidateRect(*handle(), NULL, FALSE);
	}

	return true;
}

Window* BitmapWindow::Clone() const
{
	return new BitmapWindow(*this);
}

}
