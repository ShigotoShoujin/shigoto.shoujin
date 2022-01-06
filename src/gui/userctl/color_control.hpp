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

class ColorControl : public Window {
public:
	explicit ColorControl(LayoutParam const& layout_param = {});
	virtual ~ColorControl() = default;

protected:
	virtual void BeforeCreate(CreateParam& create_param) override;
	virtual bool OnCreate(CREATESTRUCT const& createparam) override;
	virtual void OnInitialize(Window* source) override;

private:
	static Size const kDefaultClientSize;
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(LayoutParam const& layout_param);

	BitmapWindow* _gradient_map;
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

	void SetTextRGB(ColorByteRGB const& cbrgb);
	void SetTextHex(ColorByteRGB const& cbrgb);
	void SetTextHSL(ColorByteHSL const& cbhsl);

	void DrawHueBarCaret();

	static void GradientMap_OnInitialize(Window* source, void* userdata);
	static bool GradientMap_OnMouseDown(Window* source, MouseEvent const& e, void* userdata);
	static bool GradientMap_OnMouseMove(Window* source, MouseEvent const& e, void* userdata);
	static void HueBar_OnInitialize(Window* source, void* userdata);
	static bool HueBar_OnMouseDown(Window* source, MouseEvent const& e, void* userdata);
	static bool HueBar_OnMouseMove(Window* source, MouseEvent const& e, void* userdata);
	static bool NumericRGB_OnChange(EditControl* source, void* userdata);
	static bool NumericHSL_OnChange(EditControl* source, void* userdata);
};

}

#endif
