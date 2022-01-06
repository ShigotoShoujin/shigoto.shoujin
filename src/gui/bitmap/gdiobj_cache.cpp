#include "../../assert/assert.hpp"
#include "gdiobj_cache.hpp"

namespace shoujin::gui::bitmap {

GdiObjCache::Brush::Brush(Color color)
{
	_hbrush = SHOUJIN_ASSERT(CreateSolidBrush(color));
}

GdiObjCache::Brush::Brush(GdiObjCache::Brush&& other) noexcept :
	_hbrush{nullptr}
{
	swap(*this, other);
}

GdiObjCache::Brush::~Brush()
{
	if(_hbrush) {
		DeleteObject(_hbrush);
		_hbrush = nullptr;
	}
}

void swap(GdiObjCache::Brush& first, GdiObjCache::Brush& second)
{
	SHOUJIN_ASSERT(&first != &second);
	using std::swap;

	swap(first._hbrush, second._hbrush);
}

GdiObjCache::Pen::Pen(Color color)
{
	_hpen = SHOUJIN_ASSERT(CreatePen(PS_SOLID, 0, color));
}

GdiObjCache::Pen::Pen(GdiObjCache::Pen&& other) noexcept :
	_hpen{nullptr}
{
	swap(*this, other);
}

GdiObjCache::Pen::~Pen()
{
	if(_hpen) {
		DeleteObject(_hpen);
		_hpen = nullptr;
	}
}

void swap(GdiObjCache::Pen& first, GdiObjCache::Pen& second)
{
	SHOUJIN_ASSERT(&first != &second);
	using std::swap;

	swap(first._hpen, second._hpen);
}

HBRUSH GdiObjCache::GetBrush(Color const& color)
{
	auto [pair, created] = _brush_cache.try_emplace(color, color);
	return pair->second;
}

HPEN GdiObjCache::GetPen(Color const& color)
{
	auto [pair, created] = _pen_cache.try_emplace(color, color);
	return pair->second;
}

}
