#include "BitmapControl.hpp"

BitmapControl::BitmapControl(BitmapControl&& other) noexcept :
	Control{std::move(other)} {}

BitmapControl::BitmapControl(const UserControlCreateInfo& ucci) noexcept :
	Control{ucci, 0, 0, 0},
	Bitmap{Control::GetClientSize()}
{
	Bitmap::Fill(RGB(0xff, 0, 0));
}

bool BitmapControl::OnPaint() noexcept
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	int x = ps.rcPaint.left;
	int y = ps.rcPaint.top;
	int w = ps.rcPaint.right - x;
	int h = ps.rcPaint.bottom - y;
	BitBlt(ps.hdc, x, y, w, h, hdc, x, y, SRCCOPY);
	EndPaint(hwnd, &ps);
	return true;
}

void BitmapControl::SetParent(const Window& parent_window) noexcept
{
	Control::SetParent(parent_window);
	Bitmap::Reset(Control::GetClientSize());
	Bitmap::Fill(RGB(0xff, 0, 0));
}
