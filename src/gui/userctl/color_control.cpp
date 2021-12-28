#include "../../assert/assert.hpp"
#include "../bitmap/bitmap_window.hpp"
#include "../comctl32/edit_control.hpp"
#include "../comctl32/label_control.hpp"
#include "../layout/layout_stream.hpp"
#include "color_control.hpp"

using namespace shoujin::gui;
using namespace shoujin::gui::bitmap;
using namespace shoujin::gui::layout;

static void RenderGradientMap(Bitmap& bitmap, Color const& color)
{
	auto bits = bitmap.GetBits();
	bits.RenderGradientMap(Color::White, color, Color::Black, Color::Black);
	bitmap.SetBits(bits);
}

namespace shoujin::gui::comctl32 {

Size const ColorControl::kDefaultClientSize{768, 768};

ColorControl::ColorControl(LayoutParam const& layout_param) :
	Window{BuildLayout(layout_param)}
{
	LayoutParam layout{.tabstop = false};

	_gradient_map = new BitmapWindow(layout);
	_gradient_map->OnInitializeEvent = GradientMap_OnInitialize;
	_gradient_map->OnClickEvent = {GradientMap_OnClick, this};

	_gradient_bar_h = new BitmapWindow(layout);
	_gradient_bar_h->OnInitializeEvent = GradientBarH_OnInitialize;
	_gradient_bar_h->OnClickEvent = {GradientBar_OnClick, this};

	_gradient_bar_v = new BitmapWindow(layout);
	_gradient_bar_v->OnInitializeEvent = GradientBarV_OnInitialize;
	_gradient_bar_v->OnClickEvent = {GradientBar_OnClick, this};

	auto window = [](LayoutParam const& lp) -> Window* { auto p{lp}; p.tabstop=false; return new Window(p); };
	auto label = [](LayoutParam const& lp) -> Window* { return new LabelControl(lp); };
	auto edit = [](LayoutParam const& lp) -> Window* { return new EditControl(lp); };

	LayoutStream stream{this};

	stream
		<< layout::window_size(client_size() / 2) << layout::exstyle(WS_EX_CLIENTEDGE)
		<< topleft << _gradient_map
		<< push << layout::window_size({client_size().x / 2, 23}) << below << _gradient_bar_h << pop
		<< layout::window_size({23, client_size().y / 2}) << after << _gradient_bar_v
		<< layout::exstyle(0) << layout::window_size(LabelControl::DefaultSize) << unrelated << after
		<< TEXT("Red") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below
		<< TEXT("Green") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below
		<< TEXT("Blue") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below;

	AddChild(new EditControl(LayoutParam{.anchor{AnchorRight | AnchorBottom}}));
	AddChild(new EditControl(LayoutParam{.anchor{AnchorRight | AnchorTop}}));
	AddChild(new EditControl(LayoutParam{.anchor{AnchorLeft | AnchorBottom}}));
}

Window::CreateParam ColorControl::OnCreateParam()
{
	return CreateParam{.classname = TEXT("ShoujinColorControl")};
}

bool ColorControl::OnCreate(CREATESTRUCT const& createparam)
{
	return Window::OnCreate(createparam);
}

Window* ColorControl::Clone() const
{
	return new ColorControl(*this);
}

LayoutParam ColorControl::BuildLayout(LayoutParam const& layout_param)
{
	Size client_size;
	if(!layout_param.window_size && !layout_param.client_size)
		client_size = kDefaultClientSize;

	LayoutParam layout = layout_param;
	layout.client_size = client_size;
	layout.exstyle = WS_EX_CLIENTEDGE;

	return layout;
}

void ColorControl::GradientMap_OnInitialize(Window* source, void* userdata)
{
	auto self = static_cast<BitmapWindow*>(source);
	RenderGradientMap(self->bitmap(), Color::Red);
}

void ColorControl::GradientMap_OnClick(Window* source, Point const& position, void* userdata)
{
	auto self = static_cast<BitmapWindow*>(source);
	auto color = self->bitmap().GetPixelColor(position);

	tstringstream tss;
	tss << "X: " << position.x << " Y: " << position.y << '\n';
	tss << "RGB : {" << color.red() << ',' << color.green() << ',' << color.blue() << "}\n";

	auto text = tss.str();
	MessageBox(*source->handle(), text.c_str(), L"OnClick", MB_OK | MB_ICONINFORMATION);
}

void ColorControl::GradientBarH_OnInitialize(Window* source, void* userdata)
{
	auto self = static_cast<BitmapWindow*>(source);
	auto& bmp = self->bitmap();

	auto bits = bmp.GetBits();
	bits.RenderGradientBarHorizontal();
	bmp.SetBits(bits);
}

void ColorControl::GradientBarV_OnInitialize(Window* source, void* userdata)
{
	auto self = static_cast<BitmapWindow*>(source);
	auto& bmp = self->bitmap();

	auto bits = bmp.GetBits();
	bits.RenderGradientBarVertical();
	bmp.SetBits(bits);
}

void ColorControl::GradientBar_OnClick(Window* source, Point const& position, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);
	auto gradient_map = parent->_gradient_map;
	auto self = static_cast<BitmapWindow*>(source);
	auto color = self->bitmap().GetPixelColor(position);

	RenderGradientMap(gradient_map->bitmap(), color);
	gradient_map->Invalidate();
}

}
