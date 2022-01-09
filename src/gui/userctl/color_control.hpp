#ifndef SHOUJIN_SOURCE_GUI_USERCTL_COLOR_CONTROL
#define SHOUJIN_SOURCE_GUI_USERCTL_COLOR_CONTROL

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
	Event<void, GradientMap*, Color const&> OnColorChangedEvent;
	void SetHue(int hue);
	void SetColor(Color const& color);

private:
	Bitmap _caret;
	Point _selector;
	ColorByteHSV _color;

	void OnInitialize() override;
	bool OnMouseDown(MouseEvent const& e) override;
	bool OnMouseMove(MouseEvent const& e) override;

	void InitializeCaret();
	void UpdateCaret(Point const& pos);
	void DrawCaret();
	Color ColorFromPosition(Point const& pos, int hue) const;
	Point PositionFromColor(ColorFloatHSV const& color) const;
};

class HueBar : public BitmapWindow {
public:
	HueBar() = default;
	Event<void, HueBar*, int> OnHueChangedEvent;
	void SetHue(int hue);

private:
	Bitmap _caret;
	int _selector{};
	int _hue{};

	void OnInitialize() override;
	bool OnMouseDown(MouseEvent const& e) override;
	bool OnMouseMove(MouseEvent const& e) override;

	void InitializeCaret();
	void UpdateCaret(int x_pos);
	void DrawCaret();
	void SetHueBar(int x_pos, int hue);
	void SetHueBarFromPos(int x_pos);
	void SetHueBarFromHue(int hue);
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
	static Size const kDefaultClientSize;
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(LayoutParam const& layout_param);

	GradientMap* _gradient_map{};
	HueBar* _hue_bar;

	NumericControl* _numeric_red;
	NumericControl* _numeric_green;
	NumericControl* _numeric_blue;
	EditControl* _edit_hex;
	NumericControl* _numeric_hue;
	NumericControl* _numeric_saturation;
	NumericControl* _numeric_lightness;

	int _numeric_events_depth{};
	int _color_events_depth{};

	void SetText(Color const& color);
	void SetTextRGB(ColorByteRGB const& cbrgb);
	void SetTextHex(ColorByteRGB const& cbrgb);
	void SetTextHSL(ColorByteHSL const& cbhsl);

	static void GradientMap_OnColorChanged(GradientMap* source, Color const& color, void* userdata);
	static void HueBar_OnHueChanged(HueBar* source, int hue, void* userdata);

	static bool NumericRGB_OnChange(EditControl* source, void* userdata);
	static bool NumericHSL_OnChange(EditControl* source, void* userdata);
};

}

#endif
