#ifndef SHOUJIN_SOURCE_GUI_BITMAP_GDIOBJ_CACHE
#define SHOUJIN_SOURCE_GUI_BITMAP_GDIOBJ_CACHE

#include "../color.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <unordered_map>

namespace shoujin::gui::bitmap {

class GdiObjCache {
public:
	HBRUSH GetBrush(Color const& color);
	HPEN GetPen(Color const& color);
	~GdiObjCache();

private:
	std::unordered_map<COLORREF, HBRUSH> _brush_map;
	std::unordered_map<COLORREF, HPEN> _pen_map;
};

}

#endif
