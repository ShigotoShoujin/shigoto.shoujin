#include "../../assert/assert.hpp"
#include "brush_cache.hpp"

namespace shoujin::gui::bitmap {

HBRUSH BrushCache::GetBrush(Color const& color)
{
	if(_brush_map.contains(color))
		return _brush_map[color];

	HBRUSH hbrush = SHOUJIN_ASSERT(CreateSolidBrush(color));
	_brush_map[color] = hbrush;

	return hbrush;
}

BrushCache::~BrushCache()
{
	for(auto&& it : _brush_map)
		DeleteObject(it.second);
}

}
