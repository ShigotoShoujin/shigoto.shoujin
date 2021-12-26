#ifndef SHOUJIN_SOURCE_GUI_USERCTL_COLOR_CONTROL
#define SHOUJIN_SOURCE_GUI_USERCTL_COLOR_CONTROL

#include "../bitmap/bitmap.hpp"
#include "../window.hpp"
#include <memory>

namespace shoujin::gui::comctl32 {

class ColorControl : public Window {
public:
	explicit ColorControl(LayoutParam const& lp = {});
	virtual ~ColorControl() = default;

	virtual CreateParam OnCreateParam() override;
	virtual bool OnCreate(CREATESTRUCT const& createparam) override;

private:
	static Size const kDefaultClientSize;
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(LayoutParam const& lp);

	static void GradientMap_OnInitialize(Window* source, void* userdata);
	static void GradientBarH_OnInitialize(Window* source, void* userdata);
	static void GradientBarV_OnInitialize(Window* source, void* userdata);
};

}

#endif
