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
	//_bitmap->Fill(Color(160, 80, 100));

	struct crange {
		struct {
			float step;
			float val;
		} h, v;

		crange(int width, int height, uint8_t min) :
			_width{width}, _height{height}, _min{min}
		{
			v.step = _min / static_cast<float>(_height);
			v.val = 255.f;

			h.step = {};
			h.val = {};
		}

		inline void row_begin()
		{
			h.step = v.val / static_cast<float>(_width);
			h.val = v.val;
		}

		inline uint8_t next()
		{
			return static_cast<uint8_t>(h.val -= h.step);
		}

		inline void row_end()
		{
			v.val -= v.step;
		}

		int _width;
		int _height;
		uint8_t _min;
	};

	auto bits = _bitmap->GetBits();

	crange red(bits.width(), bits.height(), 255);
	crange green(bits.width(), bits.height(), 0);
	crange blue(bits.width(), bits.height(), 0);

	for(auto&& row : bits.EnumerateRows()) {
		red.row_begin();
		green.row_begin();
		blue.row_begin();

		for(auto&& pixel : row) {
			pixel.r = red.next();
			pixel.g = 0; //green.next();
			pixel.b = 0; //blue.next();
		}

		red.row_end();
		green.row_end();
		blue.row_end();
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
