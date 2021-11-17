#include "../../assert/assert.hpp"
#include "../comctl32/edit_control.hpp"
#include "../comctl32/label_control.hpp"
#include "../layout/layout_stream.hpp"
#include "color_control.hpp"

using namespace shoujin::gui;
using namespace shoujin::gui::layout;

namespace shoujin::gui::comctl32 {

const Size ColorControl::DefaultClientSize{768, 768};

ColorControl::ColorControl(const LayoutParam& lp) :
	Window{BuildLayout(lp)}
{
	auto window = [](const LayoutParam& lp) -> Window* { auto p{lp}; p.tabstop=false; return new Window(p); };
	auto label = [](const LayoutParam& lp) -> Window* { return new LabelControl(lp); };
	auto edit = [](const LayoutParam& lp) -> Window* { return new EditControl(lp); };

	LayoutStream stream;

	stream
		<< Size{client_size() / 2} << layout::exstyle(WS_EX_CLIENTEDGE)
		<< topleft << create(this, window)
		<< layout::exstyle(0) << LabelControl::DefaultSize << unrelated << after
		<< TEXT("Red") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below
		<< TEXT("Green") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below
		<< TEXT("Blue") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below;

	AddChild(new EditControl(LayoutParam{.anchor{AnchorRight | AnchorBottom}}));
	AddChild(new EditControl(LayoutParam{.anchor{AnchorRight | AnchorTop}}));
	AddChild(new EditControl(LayoutParam{.anchor{AnchorLeft | AnchorBottom}}));
}

ColorControl::ColorControl(const ColorControl& rhs)
{
	_bitmap = std::make_unique<Bitmap>(*rhs._bitmap);
}

ColorControl& ColorControl::operator=(const ColorControl& rhs)
{
	if(this != &rhs)
		_bitmap = std::make_unique<Bitmap>(*rhs._bitmap);
	return *this;
}

Window::CreateParam ColorControl::OnCreateParam()
{
	return CreateParam{.classname = TEXT("SHOUJIN_COLOR_CTRL")};
}

bool ColorControl::OnCreate(const CREATESTRUCT& createparam)
{
	_bitmap = std::make_unique<Bitmap>(client_size());
	_bitmap->Fill(Color::Lime);

	GetChild(0)->OnPaintEvent = {OnPaintHandler, this};
	return true;
}

void ColorControl::OnDestroy()
{
	GetChild(0)->OnPaintEvent = nullptr;
}

Window* ColorControl::Clone() const
{
	return new ColorControl(*this);
}

bool ColorControl::OnPaintHandler(void* userdata)
{
	ColorControl* self = SHOUJIN_ASSERT(reinterpret_cast<ColorControl*>(userdata));
	Window* child = SHOUJIN_ASSERT(self->GetChild(0));
	HWND hwnd = child->handle()->hwnd();
	HDC source_hdc = self->_bitmap->hdc();

	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	int x = ps.rcPaint.left;
	int y = ps.rcPaint.top;
	int w = ps.rcPaint.right - x;
	int h = ps.rcPaint.bottom - y;
	BitBlt(ps.hdc, x, y, w, h, source_hdc, x, y, SRCCOPY);
	EndPaint(hwnd, &ps);

	return true;
}

LayoutParam ColorControl::BuildLayout(const LayoutParam& lp)
{
	Size client_size;
	if(!lp.window_size && !lp.client_size)
		client_size = DefaultClientSize;

	LayoutParam layout = lp;
	layout.client_size = client_size;
	layout.exstyle = WS_EX_CLIENTEDGE;

	return layout;
}

}
