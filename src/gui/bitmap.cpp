#include "Bitmap.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <utility>
#include "../assert/assert.hpp"
#include "types.hpp"

using namespace shoujin::gui;

static void CreateBitmap(HDC hsourcedc, Size size, HDC& out_hdc, HBITMAP& out_hbitmap);

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

Bitmap::Bitmap(const Size& size)
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
		_size = {};
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

void Bitmap::Reset(const Size& size)
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

void Bitmap::Fill(Point position, Size size, COLORREF color)
{
	Fill({position.x, position.y, position.x + size.x, position.y + size.y}, color);
}

void Bitmap::Fill(COLORREF color)
{
	Fill({0, 0, _size.x, _size.y}, color);
}

void Bitmap::Draw(HDC source, Point position, Size size, Point src_position)
{
	BitBlt(_hdc, position.x, position.y, size.x, size.y, source, src_position.x, src_position.y, SRCCOPY);
}

void Bitmap::Draw(const Bitmap& source)
{
	int w = max(source._size.x, _size.x);
	int h = max(source._size.y, _size.y);
	Draw(source._hdc, {}, {w, h});
}

}

static void CreateBitmap(HDC hsourcedc, Size size, HDC& out_hdc, HBITMAP& out_hbitmap)
{
	SHOUJIN_ASSERT_WIN32(out_hdc = CreateCompatibleDC(hsourcedc));
	SHOUJIN_ASSERT_WIN32(out_hbitmap = CreateCompatibleBitmap(hsourcedc, size.x, size.y));

	auto isok = [](auto r) { return r && r != HGDI_ERROR; };
	SHOUJIN_ASSERT_WIN32_FUNC(SelectObject(out_hdc, out_hbitmap), isok);
}
