#ifndef SHOUJIN_SOURCE_GUI_BITMAP_GDIOBJ_CACHE
#define SHOUJIN_SOURCE_GUI_BITMAP_GDIOBJ_CACHE

#include "../color.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <unordered_map>

namespace shoujin::gui::bitmap {

class GdiObjCache {
public:
	class Brush {
	public:
		explicit Brush(Color color);
		Brush(Brush&&) noexcept;
		~Brush();
		friend void swap(Brush& first, Brush& second);
		operator HBRUSH() { return _hbrush; }

	private:
		HBRUSH _hbrush;
	};

	class Pen {
	public:
		explicit Pen(Color color);
		Pen(Pen&&) noexcept;
		~Pen();
		friend void swap(Pen& first, Pen& second);
		operator HPEN() { return _hpen; }

	private:
		HPEN _hpen;
	};

	HBRUSH GetBrush(Color const& color);
	HPEN GetPen(Color const& color);

private:
	std::unordered_map<COLORREF, Brush> _brush_cache;
	std::unordered_map<COLORREF, Pen> _pen_cache;
};

}

#endif
