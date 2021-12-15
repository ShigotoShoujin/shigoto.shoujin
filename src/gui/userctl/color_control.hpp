#ifndef SHOUJIN_SOURCE_GUI_USERCTL_COLOR_CONTROL
#define SHOUJIN_SOURCE_GUI_USERCTL_COLOR_CONTROL

#include "../bitmap.hpp"
#include "../window.hpp"
#include <memory>

namespace shoujin::gui::comctl32 {

class ColorControl : public Window {
	static const Size DefaultClientSize;

public:
	explicit ColorControl(const LayoutParam& lp = {});
	virtual ~ColorControl() = default;

	virtual CreateParam OnCreateParam() override;

private:
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(const LayoutParam& lp);
};

}

#endif
