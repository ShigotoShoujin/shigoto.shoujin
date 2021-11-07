#include "color_plane.hpp"
#include "color_picker.hpp"

namespace shoujin::gui {

ColorPicker::ColorPicker(Point position, Size window_size) :
	Window{
		CreateInfo{
			.position{position},
			.window_size{window_size},
			.exstyle{WS_EX_STATICEDGE},
		},
	}
{
	AddChild(new ColorPlane({}, {client_width() / 2, client_height()}));
}

bool ColorPicker::OnCreate(const CREATESTRUCT& createinfo)
{
	auto cs = Window::client_size();
	_bitmap.reset(new Bitmap(cs));
	_bitmap->Fill(Color::Cyan);
	_bitmap->Fill(cs / 4, cs / 2, Color::Red);
	return false;
}

bool ColorPicker::OnPaint()
{
	if(_bitmap)
		Window::ProcessOnPaintMessageFromDC(_bitmap->hdc());
	return false;
}

}
