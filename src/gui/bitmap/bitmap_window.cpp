#include "bitmap_window.hpp"

using namespace shoujin::gui;

namespace shoujin::gui::bitmap {

BitmapWindow::BitmapWindow(LayoutParam const& layout_param) :
	Window{layout_param} {}

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

void BitmapWindow::BeforeCreate(CreateParam& create_param)
{
	create_param.classname = TEXT("ShoujinBitmapWindow");
}

bool BitmapWindow::OnCreate(CREATESTRUCT const& createparam)
{
	_bitmap = std::make_unique<Bitmap>(client_size());
	return Handled;
}

bool BitmapWindow::OnPaint()
{
	HWND hwnd = this->hwnd();
	HDC source_hdc = _bitmap->hdc();

	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	int x = ps.rcPaint.left;
	int y = ps.rcPaint.top;
	int w = ps.rcPaint.right - x;
	int h = ps.rcPaint.bottom - y;
	BitBlt(ps.hdc, x, y, w, h, source_hdc, x, y, SRCCOPY);
	EndPaint(hwnd, &ps);

	return Handled;
}

bool BitmapWindow::OnSizingFinished()
{
	auto old_size = _bitmap->size();
	auto new_size = client_size();

	if(old_size != new_size) {
		_bitmap = std::make_unique<Bitmap>(client_size());
		_bitmap->Fill(Color::Lime);
		InvalidateRect(hwnd(), NULL, FALSE);
	}

	return Handled;
}

void BitmapWindow::ForceRepaint()
{
	if(hwnd())
		SendMessage(hwnd(), WM_PAINT, 0, 0);
}

Window* BitmapWindow::Clone() const
{
	return new BitmapWindow(*this);
}

}
