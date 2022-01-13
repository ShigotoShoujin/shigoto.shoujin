import ScopeGuard;

#include "../../assert/assert.hpp"
#include "../bitmap/bitmap_window.hpp"
#include "../layout/layout_stream.hpp"
#include "color_control.hpp"
#include <format>

using namespace shoujin::gui;
using namespace shoujin::gui::bitmap;
using namespace shoujin::gui::layout;

auto constexpr kGradientCaretSize = 9;
auto constexpr kHueBarHeight = 23;
auto constexpr kHueBarCaretSize = 5;
Size constexpr kDefaultClientSize{768, 768};

namespace shoujin::gui::usercontrol {

namespace internal {

void GradientMap::setHue(float hue)
{
	renderMap(hue);
	color = colorFromPosition(selector, static_cast<float>(hue));
	onColorChangedEvent(this, color);
}

void GradientMap::setColor(ColorFloatHSV const& color)
{
	selector = positionFromColor(color);
	renderMap(color.hue);
	this->color = color;
	onColorChangedEvent(this, color);
}

void GradientMap::OnInitialize()
{
	BitmapWindow::OnInitialize();
	selector = {client_size().x - 1, 0};
	initializeCaret();
	setHue(0);
}

bool GradientMap::OnMouseDown(MouseEvent const& e)
{
	if(MouseButton::None != (e.ButtonFlag ^ MouseButton::Left))
		return NotHandled;

	updateCaret(e.Position);

	color = colorFromPosition(e.Position, color.hue);
	onColorChangedEvent(this, color);

	return Handled;
}

bool GradientMap::OnMouseMove(MouseEvent const& e)
{
	if(MouseButton::None != (e.ButtonFlag ^ MouseButton::Left))
		return NotHandled;

	MouseEvent me = e;
	me.Position.ClampPoint(client_size());
	OnMouseDown(me);

	return Handled;
}

void GradientMap::initializeCaret()
{
	auto constexpr size = kGradientCaretSize;
	auto constexpr half = kGradientCaretSize / 2;

	caret.Resize({size, size});
	caret.Fill({}, {half, half}, Color::White); //TopLeft
	caret.Fill({half + 1, half + 1}, {half, half}, Color::White); //TopRight
	caret.Fill({0, half + 1}, {half, half}, Color::White); //BottomLeft
	caret.Fill({half + 1, 0}, {half, half}, Color::White); //BottomRight
	caret.DrawLine({0, 0}, {size, size}, Color::Black); //TopLeft to BottomRight
	caret.DrawLine({size - 1, 0}, {-1, size}, Color::Black); //TopRIght to BottomLeft
}

void GradientMap::updateCaret(Point const& pos)
{
	drawCaret();
	selector = pos;
	drawCaret();
	Repaint();
}

void GradientMap::drawCaret()
{
	Draw(caret, selector - (kGradientCaretSize / 2), caret.size(), {}, Bitmap::RasterMode::SrcInvert);
}

void GradientMap::renderMap(float hue)
{
	auto bits = GetBits();
	bits.RenderGradientMap(Color::White, ColorFloatHSV(hue, 1.f, 1.f), Color::Black, Color::Black);
	SetBits(bits);

	drawCaret();
	Repaint();
}

ColorFloatHSV GradientMap::colorFromPosition(Point const& pos, float hue) const
{
	auto [sx, sy] = client_size();
	auto S = pos.x / static_cast<float>(sx - 1);
	auto V = 1 - pos.y / static_cast<float>(sy - 1);
	return {hue, S, V};
}

Point GradientMap::positionFromColor(ColorFloatHSV const& color) const
{
	auto [sx, sy] = client_size();
	auto x = static_cast<int>(std::roundf(sx * color.saturation));
	auto y = static_cast<int>(std::roundf(sy * (1 - color.value)));
	return {x, y};
}

void HueBar::setHue(float hue)
{
	setHueBarFromHue(hue);
}

void HueBar::OnInitialize()
{
	BitmapWindow::OnInitialize();

	auto bits = GetBits();
	bits.RenderHueBarHorizontal();
	SetBits(bits);

	initializeCaret();
	drawCaret();
}

bool HueBar::OnMouseDown(MouseEvent const& e)
{
	if(MouseButton::None != (e.ButtonFlag ^ MouseButton::Left))
		return NotHandled;

	updateCaret(e.Position.x);
	setHueBarFromPos(e.Position.x);

	return Handled;
}

bool HueBar::OnMouseMove(MouseEvent const& e)
{
	if(MouseButton::None != (e.ButtonFlag ^ MouseButton::Left))
		return NotHandled;

	MouseEvent me = e;
	me.Position.ClampPoint(client_size());
	OnMouseDown(me);

	return Handled;
}

void HueBar::initializeCaret()
{
	auto constexpr half = kHueBarCaretSize / 2;
	auto height = client_size().y;
	caret.Resize({kHueBarCaretSize, height});
	caret.DrawLine({half, 0}, {half, height}, Color::White);
	caret.Fill({0, 0}, {kHueBarCaretSize, kHueBarCaretSize}, Color::White);
	caret.Fill({0, height - kHueBarCaretSize}, {kHueBarCaretSize, kHueBarCaretSize}, Color::White);
}

void HueBar::updateCaret(int x_pos)
{
	drawCaret();
	selector = x_pos;
	drawCaret();
	Repaint();
}

void HueBar::drawCaret()
{
	Draw(caret, {selector - kHueBarCaretSize / 2, 0}, caret.size(), {}, Bitmap::RasterMode::SrcInvert);
}

void HueBar::setHueBar(int x_pos, float hue)
{
	updateCaret(x_pos);
	OnHueChangedEvent(this, hue);
}

void HueBar::setHueBarFromPos(int x_pos)
{
	auto hue = 360 - std::roundf((360.f / (client_size().x - 1) * x_pos));
	setHueBar(x_pos, hue);
}

void HueBar::setHueBarFromHue(float hue)
{
	auto x_pos = static_cast<int>(hue / (360.f / client_size().x));
	setHueBar(x_pos, hue);
}

}

ColorControl::ColorControl(LayoutParam const& layout_param) :
	Window{buildLayout(layout_param)}
{
	gradientMap = new internal::GradientMap();
	gradientMap->onColorChangedEvent = {gradientMapColorChangedHandler, this};

	hueBar = new HueBar();
	hueBar->OnHueChangedEvent = {hueBarHueChangedHandler, this};

	preview = new BitmapWindow();

	for(auto** it : {&txtRgbR, &txtRgbG, &txtRgbB}) {
		*it = new NumericControl();
		(*it)->OnChangeEvent = {txtRgbChangeHandler, this};
		(*it)->min_value(0);
		(*it)->max_value(255);
	}

	txtHex = new EditControl();
	txtHex->autoselect(true);
	txtHex->readonly(true);

	for(auto** it : {&txtHslH, &txtHslS, &txtHslL}) {
		*it = new NumericControl();
		(*it)->OnChangeEvent = {txtHslChangeHandler, this};
		(*it)->min_value(0);
		(*it)->max_value(100);
	}

	txtHslH->min_value(0);
	txtHslH->max_value(360);

	for(auto** it : {&txtHsvH, &txtHsvS, &txtHsvV}) {
		*it = new NumericControl();
		(*it)->OnChangeEvent = {txtHsvChangeHandler, this};
		(*it)->min_value(0);
		(*it)->max_value(100);
	}

	txtHsvH->min_value(0);
	txtHsvH->max_value(360);

	auto label = [](LayoutParam const& lp) -> Window* { return new LabelControl(lp); };

	LayoutStream stream{this};

	stream
		<< layout::window_size(client_size() / 4) << layout::exstyle(WS_EX_CLIENTEDGE)
		<< topleft << gradientMap
		<< push << layout::window_size({client_size().x / 4, kHueBarHeight}) << below << hueBar
		<< below << layout::window_size({client_size().x / 4, 100}) << preview << pop
		<< layout::exstyle(0) << layout::window_size(LabelControl::DefaultSize) << unrelated << after
		<< TEXT("Red") << create(this, label) << push << after << txtRgbR << pop << below
		<< TEXT("Green") << create(this, label) << push << after << txtRgbG << pop << below
		<< TEXT("Blue") << create(this, label) << push << after << txtRgbB << pop << below
		<< TEXT("Hex") << create(this, label) << push << after << txtHex << pop << below
		<< TEXT("Hue") << create(this, label) << push << after << txtHslH << pop << below
		<< TEXT("Saturation") << create(this, label) << push << after << txtHslS << pop << below
		<< TEXT("Lightness") << create(this, label) << push << after << txtHslL << pop << below
		<< TEXT("Hue") << create(this, label) << push << after << txtHsvH << pop << below
		<< TEXT("Saturation") << create(this, label) << push << after << txtHsvS << pop << below
		<< TEXT("Value") << create(this, label) << push << after << txtHsvV << pop << below;
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
	Window::OnInitialize();
}

Window* ColorControl::Clone() const
{
	return new ColorControl(*this);
}

LayoutParam ColorControl::buildLayout(LayoutParam const& layout_param)
{
	Size client_size;
	if(!layout_param.window_size && !layout_param.client_size)
		client_size = kDefaultClientSize;

	LayoutParam layout = layout_param;
	layout.client_size = client_size;
	layout.exstyle = WS_EX_CLIENTEDGE;

	return layout;
}

void ColorControl::setText(ColorFloatHSV const& color)
{
	if(!numericEventsDepth) {
		EventDepth depth{numericEventsDepth};
		Color rgb{color};
		setTextRGB(rgb);
		setTextHex(rgb);

		ColorFloatHSL hsl{color};
		hsl.hue = color.hue;
		setTextHSL(hsl);
		setTextHSV(color);

		preview->Fill(rgb);
		preview->Repaint();
	}
}

void ColorControl::setTextRGB(ColorByteRGB const& cbrgb)
{
	txtRgbR->SetValue(cbrgb.red);
	txtRgbG->SetValue(cbrgb.green);
	txtRgbB->SetValue(cbrgb.blue);
}

void ColorControl::setTextHex(ColorByteRGB const& cbrgb)
{
	txtHex->SetText(std::format(TEXT("0x{:02x}{:02x}{:02x}"), cbrgb.red, cbrgb.green, cbrgb.blue));
}

void ColorControl::setTextHSL(ColorByteHSL const& cbhsl)
{
	txtHslH->SetValue(cbhsl.hue);
	txtHslS->SetValue(cbhsl.saturation);
	txtHslL->SetValue(cbhsl.lightness);
}

void ColorControl::setTextHSV(ColorByteHSV const& cbhsv)
{
	txtHsvH->SetValue(cbhsv.hue);
	txtHsvS->SetValue(cbhsv.saturation);
	txtHsvV->SetValue(cbhsv.value);
}

void ColorControl::gradientMapColorChangedHandler(GradientMap* source, ColorFloatHSV const& color, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);
	if(!parent->colorEventsDepth)
		parent->setText(color);
}

void ColorControl::hueBarHueChangedHandler(HueBar* source, float hue, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);
	parent->gradientMap->setHue(hue);
}

bool ColorControl::txtRgbChangeHandler(EditControl* source, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);

	if(!parent->numericEventsDepth)
		parent->syncFromRGB();

	return Handled;

	return Handled;
}

bool ColorControl::txtHslChangeHandler(EditControl* source, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);

	if(!parent->numericEventsDepth)
		parent->syncFromHSL();

	return Handled;
}

bool ColorControl::txtHsvChangeHandler(EditControl* source, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);

	if(!parent->numericEventsDepth)
		parent->syncFromHSV();

	return Handled;
}

void ColorControl::sync(ColorFloatHSV const& hsv)
{
	setText(hsv);

	hueBar->OnHueChangedEvent.pushDisable();
	auto guard = ScopeGuard<decltype(hueBar->OnHueChangedEvent)>{[](decltype(hueBar->OnHueChangedEvent)* src) { src->popDisable(); }, &hueBar->OnHueChangedEvent};
	hueBar->setHue(hsv.hue);
	gradientMap->setColor(hsv);
}

void ColorControl::syncFromRGB()
{
	auto R = txtRgbR->GetValue();
	auto G = txtRgbG->GetValue();
	auto B = txtRgbB->GetValue();

	auto hsv = ColorFloatHSV{ColorByteRGB{R, G, B}};
	sync(hsv);
}

void ColorControl::syncFromHSL()
{
	auto H = txtHslH->GetValue();
	auto S = txtHslS->GetValue();
	auto L = txtHslL->GetValue();

	auto hsv = ColorFloatHSV{ColorByteHSL{H, S, L}};
	hsv.hue = static_cast<float>(H);
	sync(hsv);
}

void ColorControl::syncFromHSV()
{
	auto H = txtHsvH->GetValue();
	auto S = txtHsvS->GetValue();
	auto V = txtHsvV->GetValue();

	auto hsv = ColorFloatHSV{ColorByteHSV{H, S, V}};
	hsv.hue = static_cast<float>(H);
	sync(hsv);
}

}
