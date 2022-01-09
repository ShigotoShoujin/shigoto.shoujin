#include "../../assert/assert.hpp"
#include "../bitmap/bitmap_window.hpp"
#include "../layout/layout_stream.hpp"
#include "color_control.hpp"
#include <algorithm>
#include <format>

using namespace shoujin::gui;
using namespace shoujin::gui::bitmap;
using namespace shoujin::gui::layout;

int constexpr kGradientCaretSize = 7;
int constexpr kHueBarHeight = 23;
int constexpr kHueBarCaretSize = 5;

namespace shoujin::gui::usercontrol {

namespace internal {

auto constexpr kGradientCaretSize = 7;

void GradientMap::SetHue(int hue)
{
	ColorByteHSL color(hue, 100, 50);

	auto bits = GetBits();
	bits.RenderGradientMap(Color::White, color, Color::Black, Color::Black);
	SetBits(bits);

	DrawCaret();
	ForceRepaint();

	_color = ColorFromPosition(_selector, hue);
	OnColorChangedEvent(this, _color);
}

void GradientMap::SetColor(Color const& color)
{
	ColorFloatHSV cfhsv(color);
	int hue = static_cast<int>(cfhsv.hue);

	auto bits = GetBits();
	bits.RenderGradientMap(Color::White, ColorByteHSL(hue, 100, 50), Color::Black, Color::Black);
	SetBits(bits);

	_selector = PositionFromColor(cfhsv);
	DrawCaret();
	ForceRepaint();

	_color = cfhsv;
	OnColorChangedEvent(this, _color);
}

void GradientMap::OnInitialize()
{
	_selector = {client_size().x, 0};
	Resize(client_size());
	InitializeCaret();
	SetHue(0);
}

bool GradientMap::OnMouseDown(MouseEvent const& e)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return NotHandled;

	UpdateCaret(e.Position);

	_color = ColorFromPosition(e.Position, _color.hue);
	OnColorChangedEvent(this, _color);

	return Handled;
}

bool GradientMap::OnMouseMove(MouseEvent const& e)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return NotHandled;

	MouseEvent copy = e;
	copy.Position.ClampPoint(client_size());
	OnMouseDown(copy);

	return Handled;
}

void GradientMap::InitializeCaret()
{
	auto constexpr size = kGradientCaretSize;
	auto constexpr half = kGradientCaretSize / 2;

	_caret.Resize({size, size});
	_caret.Fill({}, {half, half}, Color::White); //TopLeft
	_caret.Fill({half + 1, half + 1}, {half, half}, Color::White); //TopRight
	_caret.Fill({0, half + 1}, {half, half}, Color::White); //BottomLeft
	_caret.Fill({half + 1, 0}, {half, half}, Color::White); //BottomRight
	_caret.DrawLine({0, 0}, {size, size}, Color::Black); //TopLeft to BottomRight
	_caret.DrawLine({size - 1, 0}, {-1, size}, Color::Black); //TopRIght to BottomLeft
}

void GradientMap::UpdateCaret(Point const& pos)
{
	DrawCaret();
	_selector = pos;
	DrawCaret();
	ForceRepaint();
}

void GradientMap::DrawCaret()
{
	Draw(_caret, _selector - (kGradientCaretSize / 2), _caret.size(), {}, Bitmap::RasterMode::SrcInvert);
}

Color GradientMap::ColorFromPosition(Point const& pos, int hue) const
{
	auto [sx, sy] = client_size();
	auto H = static_cast<float>(hue);
	auto S = pos.x / static_cast<float>(sx - 1);
	auto V = 1 - pos.y / static_cast<float>(sy - 1);
	return ColorFloatHSV{H, S, V};
}

Point GradientMap::PositionFromColor(ColorFloatHSV const& color) const
{
	auto [sx, sy] = client_size();
	auto x = static_cast<int>(sx * color.saturation);
	auto y = static_cast<int>(sy * color.value);
	return {x, y};
}

void HueBar::SetHue(int hue)
{
	SetHueBarFromHue(hue);
}

void HueBar::OnInitialize()
{
	Resize(client_size());

	auto bits = GetBits();
	bits.RenderHueBarHorizontal();
	SetBits(bits);

	InitializeCaret();
	DrawCaret();
}

bool HueBar::OnMouseDown(MouseEvent const& e)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return NotHandled;

	UpdateCaret(e.Position.x);
	SetHueBarFromPos(e.Position.x);

	return Handled;
}

bool HueBar::OnMouseMove(MouseEvent const& e)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return NotHandled;

	MouseEvent copy = e;
	copy.Position.ClampPoint(client_size());
	OnMouseDown(copy);

	return Handled;
}

void HueBar::InitializeCaret()
{
	auto height = client_size().y;
	_caret.Resize({kHueBarCaretSize, height});
	_caret.DrawLine({kHueBarCaretSize / 2, 0}, {kHueBarCaretSize / 2, height}, Color::White);
	_caret.Fill({0, 0}, {kHueBarCaretSize, kHueBarCaretSize}, Color::White);
	_caret.Fill({0, height - kHueBarCaretSize}, {kHueBarCaretSize, kHueBarCaretSize}, Color::White);
}

void HueBar::UpdateCaret(int x_pos)
{
	DrawCaret();
	_selector = x_pos;
	DrawCaret();
	ForceRepaint();
}

void HueBar::DrawCaret()
{
	Draw(_caret, {_selector - kHueBarCaretSize / 2, 0}, _caret.size(), {}, Bitmap::RasterMode::SrcInvert);
}

void HueBar::SetHueBar(int x_pos, int hue)
{
	UpdateCaret(x_pos);
	OnHueChangedEvent(this, hue);
}

void HueBar::SetHueBarFromPos(int x_pos)
{
	auto hue = 360 - static_cast<int>(360.f / (client_size().x - 1) * x_pos);
	SetHueBar(x_pos, hue);
}

void HueBar::SetHueBarFromHue(int hue)
{
	auto x_pos = static_cast<int>(hue / (360.f / client_size().x));
	SetHueBar(x_pos, hue);
}

}

Size const ColorControl::kDefaultClientSize{768, 768};

ColorControl::ColorControl(LayoutParam const& layout_param) :
	Window{BuildLayout(layout_param)}
{
	_gradient_map = new internal::GradientMap();
	_gradient_map->OnColorChangedEvent = {GradientMap_OnColorChanged, this};

	_hue_bar = new HueBar();
	_hue_bar->OnHueChangedEvent = {HueBar_OnHueChanged, this};

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

void ColorControl::OnInitialize()
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

void ColorControl::SetText(Color const& color)
{
	if(!_numeric_events_depth) {
		EventDepth depth{_numeric_events_depth};
		SetTextRGB(color);
		SetTextHex(color);
		SetTextHSL(color);
	}
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

void ColorControl::GradientMap_OnColorChanged(GradientMap* source, Color const& color, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);

	if(!parent->_color_events_depth)
		parent->SetText(color);
}

void ColorControl::HueBar_OnHueChanged(HueBar* source, int hue, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);
	parent->_gradient_map->SetHue(hue);
}

bool ColorControl::NumericRGB_OnChange(EditControl* source, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);

	if(!parent->_numeric_events_depth) {
		EventDepth n_depth{parent->_numeric_events_depth};
		EventDepth c_depth{parent->_color_events_depth};

		ColorByteRGB cbrgb{
			parent->_numeric_red->GetValue(),
			parent->_numeric_green->GetValue(),
			parent->_numeric_blue->GetValue()};

		ColorByteHSL cbhsl = Color(cbrgb);
		parent->SetTextHex(cbrgb);
		parent->SetTextHSL(cbhsl);
		parent->_hue_bar->SetHue(cbhsl.hue);
		parent->_gradient_map->SetColor(cbrgb);
	}

	return Handled;
}

bool ColorControl::NumericHSL_OnChange(EditControl* source, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);
	auto hue = parent->_numeric_hue->GetValue();

	if(!parent->_numeric_events_depth) {
		EventDepth n_depth{parent->_numeric_events_depth};
		EventDepth c_depth{parent->_color_events_depth};

		ColorByteHSL cbhsl{
			hue,
			parent->_numeric_saturation->GetValue(),
			parent->_numeric_lightness->GetValue()};

		auto color = Color{cbhsl};
		parent->SetTextRGB(color);
		parent->SetTextHex(color);

		auto sender = static_cast<NumericControl*>(source);
		if(sender == parent->_numeric_hue)
			parent->_hue_bar->SetHue(cbhsl.hue);

		parent->_gradient_map->SetColor(color);
	}

	return Handled;
}

}
