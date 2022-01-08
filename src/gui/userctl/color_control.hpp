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
	GradientMap();
	Event<void, GradientMap*, Color const&> OnColorChangedEvent;
	void SetHue(int hue);

private:
	Bitmap _caret;
	Point _selector;
	Color _color;

	void OnInitialize() override;
	bool OnMouseDown(MouseEvent const& e) override;
	bool OnMouseMove(MouseEvent const& e) override;

	void InitializeCaret();
	void DrawCaret();
	Color ColorFromPosition(Point const& pos) const;
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

	GradientMap* _gradient_map_ctrl{};

	BitmapWindow* _hue_bar;
	Bitmap _hue_bar_caret;
	int _hue_bar_selector_position{};

	NumericControl* _numeric_red;
	NumericControl* _numeric_green;
	NumericControl* _numeric_blue;
	EditControl* _edit_hex;
	NumericControl* _numeric_hue;
	NumericControl* _numeric_saturation;
	NumericControl* _numeric_lightness;

	bool _numeric_events_enabled{true};

	void SetText(Color const& color);
	void SetTextRGB(ColorByteRGB const& cbrgb);
	void SetTextHex(ColorByteRGB const& cbrgb);
	void SetTextHSL(ColorByteHSL const& cbhsl);

	void SetHueBarFromPos(int x);
	void SetHueBarFromHue(int hue);
	void SetHueBar(int x, int hue);
	void DrawHueBarCaret();
	void UpdateHueBarCaret(int hue);

	static void GradientMap_OnColorChanged(GradientMap* source, Color const& color, void* userdata);
	static void HueBar_OnInitialize(Window* source, void* userdata);
	static bool HueBar_OnMouseDown(Window* source, MouseEvent const& e, void* userdata);
	static bool HueBar_OnMouseMove(Window* source, MouseEvent const& e, void* userdata);
	static bool NumericRGB_OnChange(EditControl* source, void* userdata);
	static bool NumericHSL_OnChange(EditControl* source, void* userdata);
};

}

#endif
