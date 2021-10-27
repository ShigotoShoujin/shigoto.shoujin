#include "Bitmap.hpp"
#include <cassert>
#include <utility>

#define ASSERT_INITIALIZED assert(hdc);

static void CreateBitmap(HDC hsourcedc, SIZE size, HDC& out_hdc, HBITMAP& out_hbitmap)
{
	assert(out_hdc = CreateCompatibleDC(hsourcedc));
	assert(out_hbitmap = CreateCompatibleBitmap(hsourcedc, size.cx, size.cy));
	SelectObject(out_hdc, out_hbitmap);
}

void swap(Bitmap& first, Bitmap& second) noexcept
{
	assert(&first != &second);
	using std::swap;

	swap(first.hdc, second.hdc);
	swap(first.hbitmap, second.hbitmap);
	swap(first.bitmap_size, second.bitmap_size);
}

Bitmap::Bitmap() noexcept :
	hdc{}, hbitmap{}, bitmap_size{} {}

Bitmap::Bitmap(const SIZE& size) noexcept
{
	Reset(size);
}

Bitmap::Bitmap(const Bitmap& other) noexcept
{
	if(other.hdc) {
		CreateBitmap(other.hdc, other.bitmap_size, hdc, hbitmap);
		bitmap_size = other.bitmap_size;
		Draw(other);
	} else {
		hdc = nullptr;
		hbitmap = nullptr;
		bitmap_size = {0};
	}
}

Bitmap& Bitmap::operator=(const Bitmap& other) noexcept
{
	Bitmap local = other;
	swap(*this, local);
	return *this;
}

Bitmap::Bitmap(Bitmap&& other) noexcept :
	Bitmap{}
{
	swap(*this, other);
}

Bitmap& Bitmap::operator=(Bitmap&& other) noexcept
{
	Bitmap local(std::move(other));
	swap(*this, local);
	return *this;
}

Bitmap::~Bitmap() noexcept
{
	Destroy();
}

void Bitmap::Destroy() noexcept
{
	if(hdc) {
		DeleteDC(hdc);
		DeleteObject(hbitmap);
		hdc = nullptr;
		hbitmap = nullptr;
		bitmap_size = {0};
	}
}

void Bitmap::Reset(const SIZE& size) noexcept
{
	Destroy();
	HDC hdesktopdc = ::GetDC(HWND_DESKTOP);
	CreateBitmap(hdesktopdc, size, hdc, hbitmap);
	ReleaseDC(HWND_DESKTOP, hdesktopdc);
	bitmap_size = size;
}

void Bitmap::Fill(const RECT& rect, COLORREF color) noexcept
{
	ASSERT_INITIALIZED
	HBRUSH brush = CreateSolidBrush(color);
	::FillRect(hdc, &rect, brush);
	DeleteObject(brush);
}

void Bitmap::Fill(POINT position, SIZE size, COLORREF color) noexcept
{
	ASSERT_INITIALIZED
	Fill({position.x, position.y, position.x + size.cx, position.y + size.cy}, color);
}

void Bitmap::Fill(COLORREF color) noexcept
{
	ASSERT_INITIALIZED
	Fill({0, 0, bitmap_size.cx, bitmap_size.cy}, color);
}

void Bitmap::Draw(HDC source, int x, int y, int w, int h, int src_x, int src_y) noexcept
{
	ASSERT_INITIALIZED
	BitBlt(hdc, x, y, w, h, source, src_x, src_y, SRCCOPY);
}

void Bitmap::Draw(const Bitmap& source) noexcept
{
	ASSERT_INITIALIZED
	int w = max(source.bitmap_size.cx, bitmap_size.cx);
	int h = max(source.bitmap_size.cy, bitmap_size.cy);
	Draw(source.hdc, 0, 0, w, h);
}
