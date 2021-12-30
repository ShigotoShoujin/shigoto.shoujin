#ifndef SHOUJIN_SOURCE_GUI_USERCTL_COLOR_CONTROL
#define SHOUJIN_SOURCE_GUI_USERCTL_COLOR_CONTROL

#include "../bitmap/bitmap.hpp"
#include "../window.hpp"
#include <memory>

using namespace shoujin::gui::bitmap;

namespace shoujin::gui::bitmap {
class BitmapWindow;
}

namespace shoujin::gui::comctl32 {

class ColorControl : public Window {
public:
	explicit ColorControl(LayoutParam const& layout_param = {});
	virtual ~ColorControl() = default;

	virtual CreateParam OnCreateParam() override;
	virtual bool OnCreate(CREATESTRUCT const& createparam) override;

private:
	static Size const kDefaultClientSize;
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(LayoutParam const& layout_param);

	BitmapWindow* _gradient_map;
	BitmapWindow* _gradient_bar_h;
	BitmapWindow* _gradient_bar_v;

	static void GradientMap_OnInitialize(Window* source, void* userdata);
	static void GradientMap_OnMouseDown(Window* source, MouseEvent const& e, void* userdata);
	static void GradientBarH_OnInitialize(Window* source, void* userdata);
	static void GradientBarV_OnInitialize(Window* source, void* userdata);
	static void GradientBar_OnMouseDown(Window* source, MouseEvent const& e, void* userdata);
	static void GradientBar_OnMouseMove(Window* source, MouseEvent const& e, void* userdata);
};

}

#endif
