#include "../../assert/assert.hpp"
#include "../bitmap/bitmap_window.hpp"
#include "../layout/layout_stream.hpp"
#include "color_control.hpp"
#include <algorithm>
#include <format>

using namespace shoujin::gui;
using namespace shoujin::gui::bitmap;
using namespace shoujin::gui::layout;

auto constexpr kGradientCaretSize = 7;
auto constexpr kHueBarHeight = 23;
auto constexpr kHueBarCaretSize = 5;
Size const kDefaultClientSize{768, 768};

namespace shoujin::gui::usercontrol {

namespace internal {

void GradientMap::setHue(float hue)
{
	auto bits = GetBits();
	bits.RenderGradientMap(Color::White, ColorFloatHSV(hue, 1.f, 1.f), Color::Black, Color::Black);
	SetBits(bits);

	drawCaret();
	ForceRepaint();

	color = colorFromPosition(selector, static_cast<float>(hue));
	onColorChangedEvent(this, color);
}

void GradientMap::setColor(ColorFloatHSV const& color)
{
	auto bits = GetBits();
	bits.RenderGradientMap(Color::White, ColorFloatHSV(color.hue, 1.f, 1.f), Color::Black, Color::Black);
	SetBits(bits);

	selector = positionFromColor(color);
	drawCaret();
	ForceRepaint();

	this->color = color;
	onColorChangedEvent(this, color);
}

void GradientMap::OnInitialize()
{
	selector = {client_size().x, 0};
	Resize(client_size());
	initializeCaret();
	setHue(0);
}

bool GradientMap::OnMouseDown(MouseEvent const& e)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return NotHandled;

	updateCaret(e.Position);

	color = colorFromPosition(e.Position, color.hue);
	onColorChangedEvent(this, color);

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
	ForceRepaint();
}

void GradientMap::drawCaret()
{
	Draw(caret, selector - (kGradientCaretSize / 2), caret.size(), {}, Bitmap::RasterMode::SrcInvert);
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
	auto x = static_cast<int>(sx * color.saturation);
	auto y = static_cast<int>(sy * (1 - color.value));
	return {x, y};
}

void HueBar::setHue(float hue)
{
	setHueBarFromHue(hue);
}

void HueBar::OnInitialize()
{
	Resize(client_size());

	auto bits = GetBits();
	bits.RenderHueBarHorizontal();
	SetBits(bits);

	initializeCaret();
	drawCaret();
}

bool HueBar::OnMouseDown(MouseEvent const& e)
{
	if(e.ButtonFlag ^ MouseButton::MouseButtonLeft)
		return NotHandled;

	updateCaret(e.Position.x);
	setHueBarFromPos(e.Position.x);

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

void HueBar::initializeCaret()
{
	auto height = client_size().y;
	caret.Resize({kHueBarCaretSize, height});
	caret.DrawLine({kHueBarCaretSize / 2, 0}, {kHueBarCaretSize / 2, height}, Color::White);
	caret.Fill({0, 0}, {kHueBarCaretSize, kHueBarCaretSize}, Color::White);
	caret.Fill({0, height - kHueBarCaretSize}, {kHueBarCaretSize, kHueBarCaretSize}, Color::White);
}

void HueBar::updateCaret(int x_pos)
{
	drawCaret();
	selector = x_pos;
	drawCaret();
	ForceRepaint();
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
	auto hue = 360 - (360.f / (client_size().x - 1) * x_pos);
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

	for(auto** it : {&txtRgbR, &txtRgbG, &txtRgbB}) {
		*it = new NumericControl();
		(*it)->autoselect(true);
		(*it)->OnChangeEvent = {txtRgbChangeHandler, this};
		(*it)->min_value(0);
		(*it)->max_value(255);
	}

	txtHex = new EditControl();
	txtHex->autoselect(true);
	txtHex->readonly(true);

	for(auto** it : {&txtHslH, &txtHslS, &txtHslL}) {
		*it = new NumericControl();
		(*it)->autoselect(true);
		(*it)->OnChangeEvent = {txtHslChangeHandler, this};
		(*it)->min_value(0);
		(*it)->max_value(100);
	}

	txtHslH->min_value(0);
	txtHslH->max_value(360);

	for(auto** it : {&txtHsvH, &txtHsvS, &txtHsvV}) {
		*it = new NumericControl();
		(*it)->autoselect(true);
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
		<< push << layout::window_size({client_size().x / 4, kHueBarHeight}) << below << hueBar << pop
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
		<< TEXT("Value") << create(this, label) << push << after << txtHsvV << pop << below		;
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
	setTextHex({});
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
		Color c{color};
		setTextRGB(c);
		setTextHex(c);
		setTextHSL(c);
		setTextHSV(c);
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
	txtHslH->SetValue(cbhsv.hue);
	txtHslS->SetValue(cbhsv.saturation);
	txtHslL->SetValue(cbhsv.value);
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

	if(!parent->numericEventsDepth) {
		EventDepth n_depth{parent->numericEventsDepth};
		EventDepth c_depth{parent->colorEventsDepth};

		Color color{ColorByteRGB{
			parent->txtRgbR->GetValue(),
			parent->txtRgbG->GetValue(),
			parent->txtRgbB->GetValue()}};

		parent->setTextHex(color);
		parent->setTextHSL(color);

		ColorFloatHSV hsv{color};
		parent->hueBar->setHue(hsv.hue);
		parent->gradientMap->setColor(hsv);
	}

	return Handled;
}

bool ColorControl::txtHslChangeHandler(EditControl* source, void* userdata)
{
	auto parent = static_cast<ColorControl*>(userdata);
	auto hue = parent->txtHslH->GetValue();

	if(!parent->numericEventsDepth) {
		EventDepth n_depth{parent->numericEventsDepth};
		EventDepth c_depth{parent->colorEventsDepth};

		ColorByteHSL cbhsl{
			hue,
			parent->txtHslS->GetValue(),
			parent->txtHslL->GetValue()};

		auto color = Color{cbhsl};
		parent->setTextRGB(color);
		parent->setTextHex(color);

		auto sender = static_cast<NumericControl*>(source);
		if(sender == parent->txtHslH)
			parent->hueBar->setHue(cbhsl.hue);

		parent->gradientMap->setColor(color);
	}

	return Handled;
}

bool ColorControl::txtHsvChangeHandler(EditControl* source, void* userdata)
{
	return Handled;
}

}
