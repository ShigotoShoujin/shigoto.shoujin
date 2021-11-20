#include "bitmap_window.hpp"

using namespace shoujin::gui;

namespace shoujin::gui {

BitmapWindow::BitmapWindow(const LayoutParam& lp) :
	Window{lp} {}

BitmapWindow::BitmapWindow(const BitmapWindow& rhs)
{
	_bitmap = std::make_unique<Bitmap>(*rhs._bitmap);
}

BitmapWindow& BitmapWindow::operator=(const BitmapWindow& rhs)
{
	if(this != &rhs)
		_bitmap = std::make_unique<Bitmap>(*rhs._bitmap);
	return *this;
}

Window::CreateParam BitmapWindow::OnCreateParam()
{
	return CreateParam{.classname = TEXT("ShoujinBitmapWindow")};
}

bool BitmapWindow::OnCreate(const CREATESTRUCT& createparam)
{
	_bitmap = std::make_unique<Bitmap>(client_size());

	auto bits = _bitmap->GetBits();

	auto width = _bitmap->size().x;
	auto step = 255.0 / width;

	auto it = bits.begin();
	auto end = bits.end();
	while(it < end) {
		auto line_end = it + width;
		auto c = 0.0;
		while(it < line_end) {
			it->r = 0;
			it->g = static_cast<uint8_t>(c += step);
			it->b = 0;
			++it;
		}
	}

	//auto z = bits.PixelRows();
	//for(auto& row : bits.PixelRows())
	//	for(auto& p : row)
	//		p.r = 100;

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
