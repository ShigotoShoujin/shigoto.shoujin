#include "../../assert/assert.hpp"
#include "gdiobj_cache.hpp"

namespace shoujin::gui::bitmap {

HBRUSH GdiObjCache::GetBrush(Color const& color)
{
	if(_brush_map.contains(color))
		return _brush_map[color];

	HBRUSH hbrush = SHOUJIN_ASSERT(CreateSolidBrush(color));
	_brush_map[color] = hbrush;

	return hbrush;
}

HPEN GdiObjCache::GetPen(Color const& color)
{
	if(_pen_map.contains(color))
		return _pen_map[color];

	HPEN hpen = SHOUJIN_ASSERT(CreatePen(PS_SOLID, 0, color));
	_pen_map[color] = hpen;

	return hpen;
}

GdiObjCache::~GdiObjCache()
{
	for(auto&& it : _brush_map)
		DeleteObject(it.second);
}

}
