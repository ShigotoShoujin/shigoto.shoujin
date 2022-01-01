#include "../../assert/assert.hpp"
#include "../bitmap/bitmap_window.hpp"
#include "../comctl32/edit_control.hpp"
#include "../comctl32/label_control.hpp"
#include "../layout/layout_stream.hpp"
#include "color_control.hpp"
#include <format>

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
	_gradient_map = new BitmapWindow();
	_gradient_map->OnInitializeEvent = GradientMap_OnInitialize;
	_gradient_map->OnMouseDownEvent = {GradientMap_OnMouseDown, this};
	_gradient_map->OnMouseMoveEvent = {GradientMap_OnMouseMove, this};

	_gradient_bar_h = new BitmapWindow();
	_gradient_bar_h->OnInitializeEvent = GradientBarH_OnInitialize;
	_gradient_bar_h->OnMouseDownEvent = {GradientBar_OnMouseDown, this};
	_gradient_bar_h->OnMouseMoveEvent = {GradientBar_OnMouseMove, this};

	_gradient_bar_v = new BitmapWindow();
	_gradient_bar_v->OnInitializeEvent = GradientBarV_OnInitialize;
	_gradient_bar_v->OnMouseDownEvent = {GradientBar_OnMouseDown, this};
	_gradient_bar_v->OnMouseMoveEvent = {GradientBar_OnMouseMove, this};

	auto label = [](LayoutParam const& lp) -> Window* { return new LabelControl(lp); };

	auto new_edit = []() {
		auto* edit = new EditControl();
		edit->selectall_on_focus(true);
		return edit;
	};

	_edit_red = new_edit();
	_edit_green = new_edit();
	_edit_blue = new_edit();
	_edit_hex = new_edit();
	_edit_hue = new_edit();
	_edit_saturation = new_edit();
	_edit_lightness = new_edit();

	LayoutStream stream{this};

	stream
		<< layout::window_size(client_size() / 4) << layout::exstyle(WS_EX_CLIENTEDGE)
		<< topleft << _gradient_map
		<< push << layout::window_size({client_size().x / 4, 23}) << below << _gradient_bar_h << pop
		<< layout::window_size({23, client_size().y / 4}) << after << _gradient_bar_v
		<< layout::exstyle(0) << layout::window_size(LabelControl::DefaultSize) << unrelated << after
		<< TEXT("Red") << create(this, label) << push << after << TEXT("0") << _edit_red << pop << below
		<< TEXT("Green") << create(this, label) << push << after << TEXT("0") << _edit_green << pop << below
		<< TEXT("Blue") << create(this, label) << push << after << TEXT("0") << _edit_blue << pop << below
		<< TEXT("Hex") << create(this, label) << push << after << TEXT("0") << _edit_hex << pop << below
		<< TEXT("Hue") << create(this, label) << push << after << TEXT("0") << _edit_hue << pop << below
		<< TEXT("Saturation") << create(this, label) << push << after << TEXT("0") << _edit_saturation << pop << below
		<< TEXT("Lightness") << create(this, label) << push << after << TEXT("0") << _edit_lightness << pop << below;

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

bool ColorControl::GradientMap_OnMouseDown(Window* source, MouseEvent const& e, void* userdata)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return false;

	auto parent = static_cast<ColorControl*>(userdata);
	auto self = static_cast<BitmapWindow*>(source);
	auto color = self->bitmap().GetPixelColor(e.Position);

	auto const fmt = TEXT("{:d}");
	ColorByteRGB cbrgb = color;
	ColorByteHSL cbhsl = color;
	parent->_edit_red->SetText(std::format(fmt, cbrgb.R));
	parent->_edit_green->SetText(std::format(fmt, cbrgb.G));
	parent->_edit_blue->SetText(std::format(fmt, cbrgb.B));
	parent->_edit_hex->SetText(std::format(TEXT("{:02X}{:02X}{:02X}"), cbrgb.R, cbrgb.G, cbrgb.B));
	parent->_edit_hue->SetText(std::format(fmt, cbhsl.H));
	parent->_edit_saturation->SetText(std::format(fmt, cbhsl.S));
	parent->_edit_lightness->SetText(std::format(fmt, cbhsl.L));

	return true;
}

bool ColorControl::GradientMap_OnMouseMove(Window* source, MouseEvent const& e, void* userdata)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return false;

	auto self = static_cast<BitmapWindow*>(source);
	auto& client_size = self->client_size();

	MouseEvent mouse_down_e = e;
	mouse_down_e.Position.ClampPoint(client_size);
	GradientMap_OnMouseDown(source, mouse_down_e, userdata);

	return true;
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

bool ColorControl::GradientBar_OnMouseDown(Window* source, MouseEvent const& e, void* userdata)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return false;

	auto parent = static_cast<ColorControl*>(userdata);
	auto gradient_map = parent->_gradient_map;
	auto self = static_cast<BitmapWindow*>(source);
	auto color = self->bitmap().GetPixelColor(e.Position);

	RenderGradientMap(gradient_map->bitmap(), color);
	gradient_map->Invalidate();

	return true;
}

bool ColorControl::GradientBar_OnMouseMove(Window* source, MouseEvent const& e, void* userdata)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return false;

	auto self = static_cast<BitmapWindow*>(source);
	auto& client_size = self->client_size();

	MouseEvent mouse_down_e = e;
	mouse_down_e.Position.ClampPoint(client_size);
	GradientBar_OnMouseDown(source, mouse_down_e, userdata);

	return true;
}

}
