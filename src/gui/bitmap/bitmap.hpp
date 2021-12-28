#ifndef SHOUJIN_SOURCE_GUI_BITMAP_BITMAP
#define SHOUJIN_SOURCE_GUI_BITMAP_BITMAP

#include "bitmap_bits.hpp"
#include "../types.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui::bitmap {

class Bitmap {
	friend void swap(Bitmap&, Bitmap&) noexcept;

	HDC _hdc;
	HBITMAP _hbitmap;
	Size _size;

	Bitmap();

public:
	Bitmap(Size const& size);

	Bitmap(Bitmap const&);
	Bitmap& operator=(Bitmap const&);
	Bitmap(Bitmap&&) noexcept;
	Bitmap& operator=(Bitmap&&) noexcept;

	virtual ~Bitmap();

	[[nodiscard]] virtual Size const& size() const { return _size; }
	[[nodiscard]] virtual HDC const& hdc() const { return _hdc; }

	virtual void Destroy() noexcept;
	virtual void Reset(Size const& size);
	virtual void Fill(RECT const& rect, Color color);
	virtual void Fill(Point position, Size size, Color color);
	virtual void Fill(Color color);
	virtual void Draw(HDC source, Point position, Size size, Point src_position = {});
	virtual void Draw(Bitmap const& source);

	virtual Color GetPixelColor(Point position);
	virtual BitmapBits GetBits() const;
	virtual void SetBits(BitmapBits const&);
};

}

#endif
