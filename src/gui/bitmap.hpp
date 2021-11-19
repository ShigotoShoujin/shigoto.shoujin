#pragma once
#include "types.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

class Bitmap {
	friend void swap(Bitmap&, Bitmap&) noexcept;

	HDC _hdc;
	HBITMAP _hbitmap;
	Size _size;

private:
	Bitmap();

public:
	Bitmap(const Size& size);

	Bitmap(const Bitmap&);
	Bitmap& operator=(const Bitmap&);
	Bitmap(Bitmap&&) noexcept;
	Bitmap& operator=(Bitmap&&) noexcept;

	virtual ~Bitmap();

	[[nodiscard]] virtual const Size& size() const { return _size; }
	[[nodiscard]] virtual const HDC& hdc() const { return _hdc; }

	virtual void Destroy() noexcept;
	virtual void Reset(const Size& size);
	virtual void Fill(const RECT& rect, Color color);
	virtual void Fill(Point position, Size size, Color color);
	virtual void Fill(Color color);
	virtual void Draw(HDC source, Point position, Size size, Point src_position = {});
	virtual void Draw(const Bitmap& source);

	void TestDiBits();
};

}
