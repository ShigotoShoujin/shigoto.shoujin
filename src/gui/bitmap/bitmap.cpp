#include "../../assert/assert.hpp"
#include "../types.hpp"
#include "bitmap.hpp"
#include <utility>

using namespace shoujin::gui;

static void CreateBitmap(HDC hsourcedc, Size size, HDC& out_hdc, HBITMAP& out_hbitmap);
static WORD const kBitmapInfoBitCount = 32;

namespace shoujin::gui::bitmap {

Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::SrcCopy{SRCCOPY};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::SrcPaint{SRCPAINT};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::SrcAnd{SRCAND};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::SrcInvert{SRCINVERT};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::SrcErase{SRCERASE};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::NotSrcCopy{NOTSRCCOPY};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::NotSrcErase{NOTSRCERASE};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::MergeCopy{MERGECOPY};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::MergePaint{MERGEPAINT};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::PatCopy{PATCOPY};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::PatPaint{PATPAINT};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::PatInvert{PATINVERT};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::DstInvert{DSTINVERT};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::Blackness{BLACKNESS};
Bitmap::RasterMode::RasterOperation Bitmap::RasterMode::Whiteness{WHITENESS};

Bitmap::Bitmap(Size const& size)
{
	Resize(size);
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

void swap(Bitmap& first, Bitmap& second) noexcept
{
	SHOUJIN_ASSERT(&first != &second);
	using std::swap;

	swap(first._hdc, second._hdc);
	swap(first._hbitmap, second._hbitmap);
	swap(first._size, second._size);
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

void Bitmap::Resize(Size const& size, bool copy)
{
	if(this->size() == size)
		return;

	if(copy)
		_ResizeCopy(size);
	else
		_ResizeClear(size);
}

void Bitmap::Fill(RECT const& rect, Color const& color)
{
	if(!*this)
		return;

	HBRUSH brush = _gdiobj_cache.GetBrush(color);
	SHOUJIN_ASSERT(::FillRect(_hdc, &rect, brush));
}

void Bitmap::Fill(Point const& position, Size const& size, Color const& color)
{
	Fill({position.x, position.y, position.x + size.x, position.y + size.y}, color);
}

void Bitmap::Fill(Color const& color)
{
	Fill({0, 0, _size.x, _size.y}, color);
}

void Bitmap::Draw(HDC source, Point const& position, Size const& size, Point const& src_position, RasterMode rop)
{
	if(!*this)
		return;

	SHOUJIN_ASSERT_WIN32(BitBlt(_hdc, position.x, position.y, size.x, size.y, source, src_position.x, src_position.y, rop));
}

void Bitmap::Draw(Bitmap const& source, Point const& position, Size const& size, Point const& src_position, RasterMode rop)
{
	Draw(source.hdc(), position, size, src_position, rop);
}

void Bitmap::Draw(Bitmap const& source)
{
	int w = max(source._size.x, _size.x);
	int h = max(source._size.y, _size.y);
	Draw(source._hdc, {}, {w, h});
}

void Bitmap::DrawLine(Point const& start, Point const& end, Color const& color)
{
	if(!*this)
		return;

	HPEN pen = _gdiobj_cache.GetPen(color);
	auto previous_obj = SelectObject(_hdc, pen);

	SHOUJIN_ASSERT(MoveToEx(_hdc, start.x, start.y, nullptr));
	SHOUJIN_ASSERT(LineTo(_hdc, end.x, end.y));

	SelectObject(_hdc, previous_obj);
}

Color Bitmap::GetPixelColor(Point const& position)
{
	if(!*this)
		return {};

	return SHOUJIN_ASSERT_WIN32_EXPLICIT(::GetPixel(_hdc, position.x, position.y), [](auto result) { return result != CLR_INVALID; });
}

BitmapBits Bitmap::GetBits() const
{
	if(!*this)
		return {};

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
	Resize({bitmap_bits.width(), bitmap_bits.height()});

	if(!*this)
		return;

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

void Bitmap::_Create(Size const& size)
{
	if(size) {
		HDC hdesktopdc = ::GetDC(HWND_DESKTOP);
		CreateBitmap(hdesktopdc, size, _hdc, _hbitmap);
		SHOUJIN_ASSERT_WIN32(ReleaseDC(HWND_DESKTOP, hdesktopdc));
		_size = size;
	}
}

void Bitmap::_ResizeClear(Size const& size)
{
	Destroy();
	_Create(size);
}

void Bitmap::_ResizeCopy(Size const& size)
{
	Bitmap backup = *this;
	Destroy();
	_Create(size);
	Draw(backup);
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
