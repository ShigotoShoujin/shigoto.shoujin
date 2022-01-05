#include "../../assert/assert.hpp"
#include "../bitmap/bitmap_window.hpp"
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

namespace shoujin::gui::usercontrol {

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

	for(auto** it : {&_numeric_red, &_numeric_green, &_numeric_blue}) {
		*it = new NumericControl();
		(*it)->autoselect(true);
		(*it)->OnChangeEvent = {NumericRGB_OnChange, this};
		(*it)->min_value(0);
		(*it)->max_value(255);
	}

	_edit_hex = new EditControl();
	_edit_hex->autoselect(true);
	_edit_hex->readonly(true);

	for(auto** it : {&_numeric_hue, &_numeric_saturation, &_numeric_lightness}) {
		*it = new NumericControl();
		(*it)->autoselect(true);
		(*it)->OnChangeEvent = {NumericHSL_OnChange, this};
		(*it)->min_value(0);
		(*it)->max_value(100);
	}

	_numeric_hue->min_value(0);
	_numeric_hue->max_value(360);

	auto label = [](LayoutParam const& lp) -> Window* { return new LabelControl(lp); };

	LayoutStream stream{this};

	stream
		<< layout::window_size(client_size() / 4) << layout::exstyle(WS_EX_CLIENTEDGE)
		<< topleft << _gradient_map
		<< push << layout::window_size({client_size().x / 4, 23}) << below << _gradient_bar_h << pop
		<< layout::window_size({23, client_size().y / 4}) << after << _gradient_bar_v
		<< layout::exstyle(0) << layout::window_size(LabelControl::DefaultSize) << unrelated << after
		<< TEXT("Red") << create(this, label) << push << after << TEXT("0") << _numeric_red << pop << below
		<< TEXT("Green") << create(this, label) << push << after << TEXT("0") << _numeric_green << pop << below
		<< TEXT("Blue") << create(this, label) << push << after << TEXT("0") << _numeric_blue << pop << below
		<< TEXT("Hex") << create(this, label) << push << after << TEXT("0") << _edit_hex << pop << below
		<< TEXT("Hue") << create(this, label) << push << after << TEXT("0") << _numeric_hue << pop << below
		<< TEXT("Saturation") << create(this, label) << push << after << TEXT("0") << _numeric_saturation << pop << below
		<< TEXT("Lightness") << create(this, label) << push << after << TEXT("0") << _numeric_lightness << pop << below;

	AddChild(new EditControl(LayoutParam{.anchor{AnchorRight | AnchorBottom}}));
	AddChild(new EditControl(LayoutParam{.anchor{AnchorRight | AnchorTop}}));
	AddChild(new EditControl(LayoutParam{.anchor{AnchorLeft | AnchorBottom}}));
}

void ColorControl::BeforeCreate(CreateParam& create_param)
{
	create_param.classname = TEXT("ShoujinColorControl");
}

bool ColorControl::OnCreate(CREATESTRUCT const& createparam)
{
	return Window::OnCreate(createparam);
}

void ColorControl::OnInitialize(Window* source)
{
	SetTextHex({});
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

void ColorControl::SetTextRGB(ColorByteRGB const& cbrgb)
{
	_numeric_red->SetValue(cbrgb.R);
	_numeric_green->SetValue(cbrgb.G);
	_numeric_blue->SetValue(cbrgb.B);
}

void ColorControl::SetTextHex(ColorByteRGB const& cbrgb)
{
	_edit_hex->SetText(std::format(TEXT("0x{:02x}{:02x}{:02x}"), cbrgb.R, cbrgb.G, cbrgb.B));
}

void ColorControl::SetTextHSL(ColorByteHSL const& cbhsl)
{
	_numeric_hue->SetValue(cbhsl.H);
	_numeric_saturation->SetValue(cbhsl.S);
	_numeric_lightness->SetValue(cbhsl.L);
}

void ColorControl::GradientMap_OnInitialize(Window* source, void* userdata)
{
	auto self = static_cast<BitmapWindow*>(source);
	RenderGradientMap(self->bitmap(), Color::Red);
}

bool ColorControl::GradientMap_OnMouseDown(Window* source, MouseEvent const& e, void* userdata)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return NotHandled;

	auto parent = static_cast<ColorControl*>(userdata);
	auto self = static_cast<BitmapWindow*>(source);
	auto color = self->bitmap().GetPixelColor(e.Position);

	ScopeFlag enabled(parent->_numeric_events_enabled, false);

	auto const fmt = TEXT("{:d}");
	parent->SetTextRGB(color);
	parent->SetTextHex(color);
	parent->SetTextHSL(color);

	return Handled;
}

bool ColorControl::GradientMap_OnMouseMove(Window* source, MouseEvent const& e, void* userdata)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return NotHandled;

	auto self = static_cast<BitmapWindow*>(source);
	auto& client_size = self->client_size();

	MouseEvent mouse_down_e = e;
	mouse_down_e.Position.ClampPoint(client_size);
	GradientMap_OnMouseDown(source, mouse_down_e, userdata);

	return Handled;
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
		return NotHandled;

	auto parent = static_cast<ColorControl*>(userdata);
	auto gradient_map = parent->_gradient_map;
	auto self = static_cast<BitmapWindow*>(source);
	auto color = self->bitmap().GetPixelColor(e.Position);

	RenderGradientMap(gradient_map->bitmap(), color);
	gradient_map->Invalidate();
	gradient_map->ForceRepaint();

	return Handled;
}

bool ColorControl::GradientBar_OnMouseMove(Window* source, MouseEvent const& e, void* userdata)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return NotHandled;

	auto self = static_cast<BitmapWindow*>(source);
	auto& client_size = self->client_size();

	MouseEvent mouse_down_e = e;
	mouse_down_e.Position.ClampPoint(client_size);
	GradientBar_OnMouseDown(source, mouse_down_e, userdata);

	return Handled;
}

bool ColorControl::NumericRGB_OnChange(EditControl* source, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);

	if(parent->_numeric_events_enabled) {
		ScopeFlag enabled{parent->_numeric_events_enabled, false};

		ColorByteRGB cbrgb{
			parent->_numeric_red->GetValue(),
			parent->_numeric_green->GetValue(),
			parent->_numeric_blue->GetValue()};

		parent->SetTextHex(cbrgb);
		parent->SetTextHSL(Color{cbrgb});
	}

	return Handled;
}

bool ColorControl::NumericHSL_OnChange(EditControl* source, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);

	if(parent->_numeric_events_enabled) {
		ScopeFlag enabled{parent->_numeric_events_enabled, false};

		ColorByteHSL cbhsl{
			parent->_numeric_hue->GetValue(),
			parent->_numeric_saturation->GetValue(),
			parent->_numeric_lightness->GetValue()};

		auto color = Color{cbhsl};
		parent->SetTextRGB(color);
		parent->SetTextHex(color);
	}

	return Handled;
}

}
