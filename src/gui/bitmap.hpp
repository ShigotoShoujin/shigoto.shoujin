#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {
class Bitmap {
	friend void swap(Bitmap&, Bitmap&) noexcept;

protected:
	HDC _hdc;
	HBITMAP _hbitmap;
	SIZE _bitmap_size;

private:
	Bitmap();

public:
	Bitmap(const SIZE& size);

	Bitmap(const Bitmap&);
	Bitmap& operator=(const Bitmap&);
	Bitmap(Bitmap&&) noexcept;
	Bitmap& operator=(Bitmap&&) noexcept;

	virtual ~Bitmap();

	[[nodiscard]] inline virtual SIZE BitmapSize() const { return _bitmap_size; }
	[[nodiscard]] inline virtual HDC HandleDC() const { return _hdc; }

	virtual void Destroy() noexcept;
	virtual void Reset(const SIZE& size);
	virtual void Fill(const RECT& rect, COLORREF color);
	virtual void Fill(POINT position, SIZE size, COLORREF color);
	virtual void Fill(COLORREF color);
	virtual void Draw(HDC source, int x, int y, int w, int h, int src_x = {}, int src_y = {});
	virtual void Draw(const Bitmap& source);
};
}