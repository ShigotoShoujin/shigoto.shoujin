#include "Bitmap.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <utility>
#include "../assert/assert.hpp"

using namespace shoujin::gui;

static void CreateBitmap(HDC hsourcedc, SIZE size, HDC& out_hdc, HBITMAP& out_hbitmap);

namespace shoujin::gui {

void swap(Bitmap& first, Bitmap& second) noexcept
{
	SHOUJIN_ASSERT(&first != &second);
	using std::swap;

	swap(first._hdc, second._hdc);
	swap(first._hbitmap, second._hbitmap);
	swap(first._size, second._size);
}

Bitmap::Bitmap() :
	_hdc{}, _hbitmap{}, _size{} {}

Bitmap::Bitmap(const SIZE& size)
{
	Reset(size);
}

Bitmap::Bitmap(const Bitmap& rhs)
{
	if(rhs._hdc) {
		CreateBitmap(rhs._hdc, rhs._size, _hdc, _hbitmap);
		_size = rhs._size;
		Draw(rhs);
	} else {
		_hdc = nullptr;
		_hbitmap = nullptr;
		_size = {0};
	}
}

Bitmap& Bitmap::operator=(const Bitmap& rhs)
{
	Bitmap local = rhs;
	swap(*this, local);
	return *this;
}

Bitmap::Bitmap(Bitmap&& rhs) noexcept :
	Bitmap{}
{
	swap(*this, rhs);
}

Bitmap& Bitmap::operator=(Bitmap&& rhs) noexcept
{
	Bitmap local(std::move(rhs));
	swap(*this, local);
	return *this;
}

Bitmap::~Bitmap()
{
	Destroy();
}

void Bitmap::Destroy() noexcept
{
	if(_hdc) {
		DeleteDC(_hdc);
		DeleteObject(_hbitmap);
		_hdc = nullptr;
		_hbitmap = nullptr;
		_size = {};
	}
}

void Bitmap::Reset(const SIZE& size)
{
	Destroy();
	HDC hdesktopdc = ::GetDC(HWND_DESKTOP);
	CreateBitmap(hdesktopdc, size, _hdc, _hbitmap);
	ReleaseDC(HWND_DESKTOP, hdesktopdc);
	_size = size;
}

void Bitmap::Fill(const RECT& rect, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	::FillRect(_hdc, &rect, brush);
	DeleteObject(brush);
}

void Bitmap::Fill(POINT position, SIZE size, COLORREF color)
{
	Fill({position.x, position.y, position.x + size.cx, position.y + size.cy}, color);
}

void Bitmap::Fill(COLORREF color)
{
	Fill({0, 0, _size.cx, _size.cy}, color);
}

void Bitmap::Draw(HDC source, int x, int y, int w, int h, int src_x, int src_y)
{
	BitBlt(_hdc, x, y, w, h, source, src_x, src_y, SRCCOPY);
}

void Bitmap::Draw(const Bitmap& source)
{
	int w = max(source._size.cx, _size.cx);
	int h = max(source._size.cy, _size.cy);
	Draw(source._hdc, 0, 0, w, h);
}

}

static void CreateBitmap(HDC hsourcedc, SIZE size, HDC& out_hdc, HBITMAP& out_hbitmap)
{
	SHOUJIN_ASSERT_WIN32(out_hdc = CreateCompatibleDC(hsourcedc));
	SHOUJIN_ASSERT_WIN32(out_hbitmap = CreateCompatibleBitmap(hsourcedc, size.cx, size.cy));

	auto isok = [](auto r) { return r && r != HGDI_ERROR; };
	SHOUJIN_ASSERT_WIN32_FUNC(SelectObject(out_hdc, out_hbitmap), isok);
}
