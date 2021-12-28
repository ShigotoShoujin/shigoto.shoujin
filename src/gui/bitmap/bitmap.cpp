#include "../../assert/assert.hpp"
#include "bitmap.hpp"
#include "../types.hpp"
#include <utility>

using namespace shoujin::gui;

static void CreateBitmap(HDC hsourcedc, Size size, HDC& out_hdc, HBITMAP& out_hbitmap);
static WORD const kBitmapInfoBitCount = 32;

namespace shoujin::gui::bitmap {

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

Bitmap::Bitmap(Size const& size)
{
	Reset(size);
}

Bitmap::Bitmap(Bitmap const& rhs)
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

Bitmap& Bitmap::operator=(Bitmap const& rhs)
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
		SHOUJIN_ASSERT_WIN32(DeleteDC(_hdc));
		SHOUJIN_ASSERT_WIN32(DeleteObject(_hbitmap));
		_hdc = nullptr;
		_hbitmap = nullptr;
		_size = {};
	}
}

void Bitmap::Reset(Size const& size)
{
	SHOUJIN_ASSERT(size);
	Destroy();
	HDC hdesktopdc = ::GetDC(HWND_DESKTOP);
	CreateBitmap(hdesktopdc, size, _hdc, _hbitmap);
	SHOUJIN_ASSERT_WIN32(ReleaseDC(HWND_DESKTOP, hdesktopdc));
	_size = size;
}

void Bitmap::Fill(RECT const& rect, Color color)
{
	HBRUSH brush = SHOUJIN_ASSERT_WIN32(CreateSolidBrush(color));
	SHOUJIN_ASSERT_WIN32(::FillRect(_hdc, &rect, brush));
	SHOUJIN_ASSERT_WIN32(DeleteObject(brush));
}

void Bitmap::Fill(Point position, Size size, Color color)
{
	Fill({position.x, position.y, position.x + size.x, position.y + size.y}, color);
}

void Bitmap::Fill(Color color)
{
	Fill({0, 0, _size.x, _size.y}, color);
}

void Bitmap::Draw(HDC source, Point position, Size size, Point src_position)
{
	BitBlt(_hdc, position.x, position.y, size.x, size.y, source, src_position.x, src_position.y, SRCCOPY);
}

void Bitmap::Draw(Bitmap const& source)
{
	int w = max(source._size.x, _size.x);
	int h = max(source._size.y, _size.y);
	Draw(source._hdc, {}, {w, h});
}

Color Bitmap::GetPixelColor(Point position)
{
	COLORREF color = GetPixel(_hdc, position.x, position.y);
	return {
		GetRValue(color),
		GetGValue(color),
		GetBValue(color)};
}

BitmapBits Bitmap::GetBits() const
{
	SHOUJIN_ASSERT(sizeof(BitmapBits::value_type) == kBitmapInfoBitCount >> 3);

	BITMAPINFO bi{};
	BITMAPINFOHEADER& bih = bi.bmiHeader;
	bih.biSize = sizeof(bih);
	auto getdibits_result = GetDIBits(_hdc, _hbitmap, 0, 0, nullptr, &bi, DIB_RGB_COLORS);
	SHOUJIN_ASSERT_WIN32(getdibits_result && getdibits_result != ERROR_INVALID_PARAMETER);

	BitmapBits bits(bih.biWidth, bih.biHeight);

	bih.biHeight = -bih.biHeight;
	bih.biBitCount = kBitmapInfoBitCount;
	bih.biCompression = BI_RGB;
	getdibits_result = GetDIBits(_hdc, _hbitmap, 0, -bih.biHeight, bits.data(), &bi, DIB_RGB_COLORS);
	SHOUJIN_ASSERT_WIN32(getdibits_result && getdibits_result != ERROR_INVALID_PARAMETER);

	return bits;
}

void Bitmap::SetBits(BitmapBits const& bitmap_bits)
{
	SHOUJIN_ASSERT(sizeof(BitmapBits::value_type) == kBitmapInfoBitCount >> 3);

	BITMAPINFO bi{};
	BITMAPINFOHEADER& bih = bi.bmiHeader;
	bih.biSize = sizeof(bih);
	bih.biWidth = bitmap_bits.width();
	bih.biHeight = -bitmap_bits.height();
	bih.biPlanes = 1;
	bih.biBitCount = kBitmapInfoBitCount;
	bih.biCompression = BI_RGB;

	SHOUJIN_ASSERT_WIN32(SetDIBits(_hdc, _hbitmap, 0, -bih.biHeight, bitmap_bits.data(), &bi, DIB_RGB_COLORS));
}

}

static void CreateBitmap(HDC hsourcedc, Size size, HDC& out_hdc, HBITMAP& out_hbitmap)
{
	SHOUJIN_ASSERT(size);
	out_hdc = SHOUJIN_ASSERT_WIN32(CreateCompatibleDC(hsourcedc));
	out_hbitmap = SHOUJIN_ASSERT_WIN32(CreateCompatibleBitmap(hsourcedc, size.x, size.y));

	auto isok = [](auto r) { return r && r != HGDI_ERROR; };
	SHOUJIN_ASSERT_WIN32_EXPLICIT(SelectObject(out_hdc, out_hbitmap), isok);
}
