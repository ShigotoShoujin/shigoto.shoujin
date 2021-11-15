#include "rect.hpp"

namespace shoujin::gui {

void Rect::SetMaxWidthUsingPosition(int max)
{
	if(width() > max)
		x1 += width() - max;
}

void Rect::SetMaxHeightUsingPosition(int max)
{
	if(height() > max)
		y1 += height() - max;
}

void Rect::SetMinWidthUsingPosition(int min)
{
	if(width() < min)
		x1 -= min - width();
}

void Rect::SetMinHeightUsingPosition(int min)
{
	if(height() < min)
		y1 -= min - height();
}

void Rect::SetMaxSizeUsingPosition(const Size& max)
{
	SetMaxWidthUsingPosition(max.x);
	SetMaxHeightUsingPosition(max.y);
}

void Rect::SetMinSizeUsingPosition(const Size& min)
{
	SetMinWidthUsingPosition(min.x);
	SetMinHeightUsingPosition(min.y);
}

void Rect::SetMaxWidthUsingSize(int max)
{
	if(width() > max)
		x2 -= width() - max;
}

void Rect::SetMaxHeightUsingSize(int max)
{
	if(height() > max)
		y2 -= height() - max;
}

void Rect::SetMinWidthUsingSize(int min)
{
	if(width() < min)
		x2 += min - width();
}

void Rect::SetMinHeightUsingSize(int min)
{
	if(height() < min)
		y2 += min - height();
}

void Rect::SetMaxSizeUsingSize(const Size& max)
{
	SetMaxWidthUsingSize(max.x);
	SetMaxHeightUsingSize(max.y);
}

void Rect::SetMinSizeUsingSize(const Size& min)
{
	SetMinWidthUsingSize(min.x);
	SetMinHeightUsingSize(min.y);
}

}
