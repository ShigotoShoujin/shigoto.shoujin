#include "bitmap_window.hpp"

using namespace shoujin::gui;

namespace shoujin::gui::bitmap {

BitmapWindow::BitmapWindow(LayoutParam const& layout_param) :
	Window{layout_param} 
{}

void BitmapWindow::BeforeCreate(CreateParam& create_param)
{
	create_param.classname = TEXT("ShoujinBitmapWindow");
}

void BitmapWindow::OnInitialize()
{
	Resize(client_size());
}

bool BitmapWindow::OnPaint()
{
	HWND hwnd = this->hwnd();

	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	int x = ps.rcPaint.left;
	int y = ps.rcPaint.top;
	int w = ps.rcPaint.right - x;
	int h = ps.rcPaint.bottom - y;
	BitBlt(ps.hdc, x, y, w, h, hdc(), x, y, SRCCOPY);
	EndPaint(hwnd, &ps);

	return Handled;
}

bool BitmapWindow::OnSizingFinished()
{
	auto new_size = client_size();

	if(size() != new_size) {
		Resize(new_size);
		Invalidate();
	}

	return Handled;
}

void BitmapWindow::Repaint()
{
	if(hwnd()) {
		Invalidate();
		SendMessage(hwnd(), WM_PAINT, 0, 0);
	}
}

Window* BitmapWindow::Clone() const
{
	return new BitmapWindow(*this);
}

}
