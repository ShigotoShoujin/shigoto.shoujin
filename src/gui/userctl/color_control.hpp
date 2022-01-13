#ifndef SHOUJIN_SOURCE_GUI_USERCTLcolor_CONTROL
#define SHOUJIN_SOURCE_GUI_USERCTLcolor_CONTROL

#include "../bitmap/bitmap.hpp"
#include "../comctl32/label_control.hpp"
#include "../comctl32/numeric_control.hpp"
#include "../window.hpp"
#include <memory>

using namespace shoujin::gui::bitmap;
using namespace shoujin::gui::comctl32;

namespace shoujin::gui::bitmap {
class BitmapWindow;
}

namespace shoujin::gui::usercontrol {

namespace internal {

class GradientMap : public BitmapWindow {
public:
	GradientMap() = default;
	Event<void, GradientMap*, ColorFloatHSV const&> onColorChangedEvent;
	void setHue(float hue);
	void setColor(ColorFloatHSV const& color);

private:
	Bitmap caret;
	Point selector;
	ColorFloatHSV color;

	void OnInitialize() override;
	void OnReady() override;
	bool OnMouseDown(MouseEvent const& e) override;
	bool OnMouseMove(MouseEvent const& e) override;

	void initializeCaret();
	void updateCaret(Point const& pos);
	void drawCaret();
	void renderMap(float hue);
	ColorFloatHSV colorFromPosition(Point const& pos, float hue) const;
	Point positionFromColor(ColorFloatHSV const& color) const;
};

class HueBar : public BitmapWindow {
public:
	HueBar() = default;
	Event<void, HueBar*, float> OnHueChangedEvent;
	void setHue(float hue);

private:
	Bitmap caret;
	int selector{};
	int hue{};

	void OnInitialize() override;
	bool OnMouseDown(MouseEvent const& e) override;
	bool OnMouseMove(MouseEvent const& e) override;

	void initializeCaret();
	void updateCaret(int x_pos);
	void drawCaret();
	void setHueBar(int x_pos, float hue);
	void setHueBarFromPos(int x_pos);
	void setHueBarFromHue(float hue);
};

}

using namespace internal;

class ColorControl : public Window {
public:
	explicit ColorControl(LayoutParam const& layout_param = {});
	virtual ~ColorControl() = default;

protected:
	virtual void BeforeCreate(CreateParam& create_param) override;
	virtual bool OnCreate(CREATESTRUCT const& createparam) override;
	virtual void OnInitialize() override;

private:
	virtual Window* Clone() const override;
	static LayoutParam buildLayout(LayoutParam const& layout_param);

	GradientMap* gradientMap{};
	HueBar* hueBar{};
	BitmapWindow* preview{};

	NumericControl* txtRgbR;
	NumericControl* txtRgbG;
	NumericControl* txtRgbB;
	EditControl* txtHex;
	NumericControl* txtHslH;
	NumericControl* txtHslS;
	NumericControl* txtHslL;
	NumericControl* txtHsvH;
	NumericControl* txtHsvS;
	NumericControl* txtHsvV;

	int numericEventsDepth{};
	int colorEventsDepth{};

	void setText(ColorFloatHSV const& color);
	void setTextRGB(ColorByteRGB const& cbrgb);
	void setTextHex(ColorByteRGB const& cbrgb);
	void setTextHSL(ColorByteHSL const& cbhsl);
	void setTextHSV(ColorByteHSV const& cbhsl);

	static void gradientMapColorChangedHandler(GradientMap* source, ColorFloatHSV const& color, void* userdata);
	static void hueBarHueChangedHandler(HueBar* source, float hue, void* userdata);

	static bool txtRgbChangeHandler(EditControl* source, void* userdata);
	static bool txtHslChangeHandler(EditControl* source, void* userdata);
	static bool txtHsvChangeHandler(EditControl* source, void* userdata);

	void sync(ColorFloatHSV const& hsv);
	void syncFromRGB();
	void syncFromHSL();
	void syncFromHSV();
};

}

#endif
