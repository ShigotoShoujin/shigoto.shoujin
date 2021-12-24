#include "bitmap_window.hpp"

using namespace shoujin::gui;

namespace shoujin::gui::bitmap {

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
