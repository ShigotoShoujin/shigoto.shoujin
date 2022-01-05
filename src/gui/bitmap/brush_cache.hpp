#ifndef SHOUJIN_SOURCE_GUI_BITMAP_BRUSHCACHE
#define SHOUJIN_SOURCE_GUI_BITMAP_BRUSHCACHE

#include "../color.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <unordered_map>

namespace shoujin::gui::bitmap {

class BrushCache {
	using BrushMap = std::unordered_map<COLORREF, HBRUSH>;

public:
	HBRUSH GetBrush(Color const& color);
	[[nodiscard]] BrushMap const& GetCache() const { return _brush_map; }
	~BrushCache();

private:
	BrushMap _brush_map;
};

}

#endif
