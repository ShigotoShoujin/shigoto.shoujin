#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Bitmap {
	friend void swap(Bitmap&, Bitmap&) noexcept;

protected:
	HDC hdc;
	HBITMAP hbitmap;
	SIZE bitmap_size;

public:
	Bitmap() noexcept;
	Bitmap(const SIZE& size) noexcept;

	Bitmap(const Bitmap&) noexcept;
	Bitmap& operator=(Bitmap) noexcept;
	Bitmap(Bitmap&&) noexcept;
	//Bitmap& operator=(Bitmap&&) noexcept;
	Bitmap& operator=(Bitmap&&) noexcept;

	virtual ~Bitmap() noexcept;

	[[nodiscard]] inline virtual SIZE GetSize() const noexcept { return bitmap_size; }
	[[nodiscard]] inline virtual HDC GetDC() const noexcept { return hdc; }

	virtual void Clear(COLORREF color);
	virtual void Fill(const RECT& rect, COLORREF color);
	virtual void Fill(POINT position, SIZE size, COLORREF color);
	virtual void Draw(HDC source, int x, int y, int w, int h, int src_x = {}, int src_y = {});
	virtual void Draw(const Bitmap& source);
};
