#ifndef SHOUJIN_SOURCE_GUI_BITMAP_BITMAP
#define SHOUJIN_SOURCE_GUI_BITMAP_BITMAP

#include "../types.hpp"
#include "bitmap_bits.hpp"
#include "gdiobj_cache.hpp"
#include <cstdint>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui::bitmap {

class Bitmap {
public:
	class RasterMode {
		struct RasterOperation {
			uint32_t value;
		};

	public:
		RasterMode(RasterOperation rop = {SrcCopy}) :
			_rop{rop} {}

		RasterMode& operator=(RasterOperation rop)
		{
			_rop = rop;
		}

		operator uint32_t() const
		{
			return _rop.value;
		}

		static RasterOperation SrcCopy; // dest = source
		static RasterOperation SrcPaint; // dest = source OR dest
		static RasterOperation SrcAnd; // dest = source AND dest
		static RasterOperation SrcInvert; // dest = source XOR dest
		static RasterOperation SrcErase; // dest = source AND (NOT dest )
		static RasterOperation NotSrcCopy; // dest = (NOT source)
		static RasterOperation NotSrcErase; // dest = (NOT src) AND (NOT dest)
		static RasterOperation MergeCopy; // dest = (source AND pattern)
		static RasterOperation MergePaint; // dest = (NOT source) OR dest
		static RasterOperation PatCopy; // dest = pattern
		static RasterOperation PatPaint; // dest = DPSnoo
		static RasterOperation PatInvert; // dest = pattern XOR dest
		static RasterOperation DstInvert; // dest = (NOT dest)
		static RasterOperation Blackness; // dest = BLACK
		static RasterOperation Whiteness; // dest = WHITE

	private:
		RasterOperation _rop;
	};

	Bitmap() = default;
	Bitmap(Size const& size);

	Bitmap(Bitmap const&);
	Bitmap& operator=(Bitmap const&);
	Bitmap(Bitmap&&) noexcept;
	Bitmap& operator=(Bitmap&&) noexcept;

	virtual ~Bitmap();
	friend void swap(Bitmap&, Bitmap&) noexcept;

	[[nodiscard]] virtual Size const& size() const { return _size; }
	[[nodiscard]] virtual HDC const& hdc() const { return _hdc; }
	[[nodiscard]] virtual operator bool() const { return _hdc; }

	virtual void Destroy() noexcept;
	virtual void Resize(Size const& size, bool copy = false);
	virtual void Fill(RECT const& rect, Color const& color);
	virtual void Fill(Point const& position, Size const& size, Color const& color);
	virtual void Fill(Color const& color);
	virtual void Draw(HDC source, Point const& position, Size const& size, Point const& src_position = {}, RasterMode rop = {});
	virtual void Draw(Bitmap const& source, Point const& position, Size const& size, Point const& src_position = {}, RasterMode rop = {});
	virtual void Draw(Bitmap const& source);
	virtual void DrawLine(Point const& start, Point const& end, Color const& color);
	virtual Color GetPixelColor(Point const& position);
	virtual BitmapBits GetBits() const;
	virtual void SetBits(BitmapBits const&);

private:
	HDC _hdc{};
	HBITMAP _hbitmap{};
	Size _size{};
	GdiObjCache _gdiobj_cache{};

	void _Create(Size const& size);
	void _ResizeClear(Size const& size);
	void _ResizeCopy(Size const& size);
};

}

#endif
