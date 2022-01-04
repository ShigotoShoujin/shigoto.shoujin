#ifndef SHOUJIN_SOURCE_GUI_USERCTL_COLOR_CONTROL
#define SHOUJIN_SOURCE_GUI_USERCTL_COLOR_CONTROL

#include "../bitmap/bitmap.hpp"
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

	virtual void BeforeCreate(CreateParam& create_param) override;
	virtual bool OnCreate(CREATESTRUCT const& createparam) override;

private:
	static Size const kDefaultClientSize;
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(LayoutParam const& layout_param);

	BitmapWindow* _gradient_map;
	BitmapWindow* _gradient_bar_h;
	BitmapWindow* _gradient_bar_v;

	EditControl* _edit_red;
	EditControl* _edit_green;
	EditControl* _edit_blue;
	EditControl* _edit_hex;
	EditControl* _edit_hue;
	EditControl* _edit_saturation;
	EditControl* _edit_lightness;

	void SetTextRGB(ColorByteRGB const& cbrgb);
	void SetTextHSL(ColorByteHSL const& cbhsl);

	static void GradientMap_OnInitialize(Window* source, void* userdata);
	static bool GradientMap_OnMouseDown(Window* source, MouseEvent const& e, void* userdata);
	static bool GradientMap_OnMouseMove(Window* source, MouseEvent const& e, void* userdata);
	static void GradientBarH_OnInitialize(Window* source, void* userdata);
	static void GradientBarV_OnInitialize(Window* source, void* userdata);
	static bool GradientBar_OnMouseDown(Window* source, MouseEvent const& e, void* userdata);
	static bool GradientBar_OnMouseMove(Window* source, MouseEvent const& e, void* userdata);
	static bool EditRGB_OnChange(EditControl* source, void* userdata);
};

}

#endif
