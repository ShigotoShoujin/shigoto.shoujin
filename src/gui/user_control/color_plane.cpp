#include "color_plane.hpp"

namespace shoujin::gui {

ColorPlane::ColorPlane(Point position, Size window_size) :
	Window{
		LayoutInfo{
			.position{position},
			.window_size{window_size},
			.exstyle{WS_EX_STATICEDGE},
		},
	}
{}

bool ColorPlane::OnCreate(const CREATESTRUCT& createinfo)
{
	auto cs = Window::client_size();
	_bitmap.reset(new Bitmap(cs));
	_bitmap->Fill(Color::Purple);
	return false;
}

bool ColorPlane::OnPaint()
{
	if(_bitmap)
		Window::ProcessOnPaintUsingDC(_bitmap->hdc());
	return false;
}

}
