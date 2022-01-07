#include "../../assert/assert.hpp"
#include "../bitmap/bitmap_window.hpp"
#include "../layout/layout_stream.hpp"
#include "color_control.hpp"
#include <algorithm>
#include <format>

using namespace shoujin::gui;
using namespace shoujin::gui::bitmap;
using namespace shoujin::gui::layout;

static int const kHueBarHeight = 23;
static int const kHueBarCaretSize = 5;

static void RenderGradientMap(Bitmap& target, Color const& color)
{
	auto bits = target.GetBits();
	bits.RenderGradientMap(Color::White, color, Color::Black, Color::Black);
	target.SetBits(bits);
}

namespace shoujin::gui::usercontrol {

Size const ColorControl::kDefaultClientSize{768, 768};

ColorControl::ColorControl(LayoutParam const& layout_param) :
	Window{BuildLayout(layout_param)},
	_hue_bar_caret{{kHueBarCaretSize, kHueBarHeight}}
{
	_gradient_map = new BitmapWindow();
	_gradient_map->OnInitializeEvent = {GradientMap_OnInitialize, this};
	_gradient_map->OnMouseDownEvent = {GradientMap_OnMouseDown, this};
	_gradient_map->OnMouseMoveEvent = {GradientMap_OnMouseMove, this};

	_hue_bar = new BitmapWindow();
	_hue_bar->OnInitializeEvent = {HueBar_OnInitialize, this};
	_hue_bar->OnMouseDownEvent = {HueBar_OnMouseDown, this};
	_hue_bar->OnMouseMoveEvent = {HueBar_OnMouseMove, this};

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
		<< push << layout::window_size({client_size().x / 4, kHueBarHeight}) << below << _hue_bar << pop
		<< layout::exstyle(0) << layout::window_size(LabelControl::DefaultSize) << unrelated << after
		<< TEXT("Red") << create(this, label) << push << after << TEXT("0") << _numeric_red << pop << below
		<< TEXT("Green") << create(this, label) << push << after << TEXT("0") << _numeric_green << pop << below
		<< TEXT("Blue") << create(this, label) << push << after << TEXT("0") << _numeric_blue << pop << below
		<< TEXT("Hex") << create(this, label) << push << after << TEXT("0") << _edit_hex << pop << below
		<< TEXT("Hue") << create(this, label) << push << after << TEXT("0") << _numeric_hue << pop << below
		<< TEXT("Saturation") << create(this, label) << push << after << TEXT("0") << _numeric_saturation << pop << below
		<< TEXT("Lightness") << create(this, label) << push << after << TEXT("0") << _numeric_lightness << pop << below;
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
	_numeric_red->SetValue(cbrgb.red);
	_numeric_green->SetValue(cbrgb.green);
	_numeric_blue->SetValue(cbrgb.blue);
}

void ColorControl::SetTextHex(ColorByteRGB const& cbrgb)
{
	_edit_hex->SetText(std::format(TEXT("0x{:02x}{:02x}{:02x}"), cbrgb.red, cbrgb.green, cbrgb.blue));
}

void ColorControl::SetTextHSL(ColorByteHSL const& cbhsl)
{
	_numeric_hue->SetValue(cbhsl.hue);
	_numeric_saturation->SetValue(cbhsl.saturation);
	_numeric_lightness->SetValue(cbhsl.lightness);
}

void ColorControl::SetHueBarFromPos(int x_pos)
{
	auto hue = 360 - static_cast<int>(360.f / _hue_bar->client_size().x * x_pos);
	SHOUJIN_ASSERT(hue >= 0 && hue <= 360);
	SetHueBar(x_pos, hue);
}

void ColorControl::SetHueBarFromHue(int hue)
{
	auto x_pos = static_cast<int>(hue / (360.f / _hue_bar->client_size().x));
	SetHueBar(x_pos, hue);
}

void ColorControl::SetHueBar(int x_pos, int hue)
{
	UpdateHueBarCaret(x_pos);
	_hue_bar->ForceRepaint();

	RenderGradientMap(_gradient_map->bitmap(), ColorByteHSL{hue, 100, 50});
	_gradient_map->ForceRepaint();
}

void ColorControl::DrawHueBarCaret()
{
	_hue_bar->bitmap().Draw(_hue_bar_caret, {_hue_bar_selector_position - kHueBarCaretSize / 2, 0}, _hue_bar_caret.size(), {}, Bitmap::RasterMode::SrcInvert);
}

void ColorControl::UpdateHueBarCaret(int hue)
{
	DrawHueBarCaret();
	_hue_bar_selector_position = hue;
	DrawHueBarCaret();
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

void ColorControl::HueBar_OnInitialize(Window* source, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);
	auto self = static_cast<BitmapWindow*>(source);
	auto& bmp = self->bitmap();

	auto bits = bmp.GetBits();
	bits.RenderHueBarHorizontal();
	bmp.SetBits(bits);

	auto height = parent->_hue_bar->client_size().y;
	parent->_hue_bar_caret.DrawLine({kHueBarCaretSize / 2, 0}, {kHueBarCaretSize / 2, height}, Color::White);
	parent->_hue_bar_caret.Fill({0, 0}, {kHueBarCaretSize, kHueBarCaretSize}, Color::White);
	parent->_hue_bar_caret.Fill({0, height - kHueBarCaretSize}, {kHueBarCaretSize, kHueBarCaretSize}, Color::White);

	parent->DrawHueBarCaret();
}

bool ColorControl::HueBar_OnMouseDown(Window* source, MouseEvent const& e, void* userdata)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return NotHandled;

	auto parent = static_cast<ColorControl*>(userdata);
	parent->SetHueBarFromPos(e.Position.x);

	return Handled;
}

bool ColorControl::HueBar_OnMouseMove(Window* source, MouseEvent const& e, void* userdata)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return NotHandled;

	auto self = static_cast<BitmapWindow*>(source);
	auto& client_size = self->client_size();

	MouseEvent mouse_down_e = e;
	mouse_down_e.Position.ClampPoint(client_size);
	HueBar_OnMouseDown(source, mouse_down_e, userdata);

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
	auto hue = parent->_numeric_hue->GetValue();

	if(parent->_numeric_events_enabled) {
		ScopeFlag enabled{parent->_numeric_events_enabled, false};

		ColorByteHSL cbhsl{
			hue,
			parent->_numeric_saturation->GetValue(),
			parent->_numeric_lightness->GetValue()};

		auto color = Color{cbhsl};
		parent->SetTextRGB(color);
		parent->SetTextHex(color);

		auto sender = static_cast<NumericControl*>(source);
		if(sender == parent->_numeric_hue)
			parent->SetHueBarFromHue(hue);
	}

	return Handled;
}

}
