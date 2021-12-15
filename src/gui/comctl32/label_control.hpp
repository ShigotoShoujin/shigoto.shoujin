#ifndef SHOUJIN_SOURCE_GUI_COMCTL32_LABEL_CONTROL
#define SHOUJIN_SOURCE_GUI_COMCTL32_LABEL_CONTROL

#include "../window.hpp"

namespace shoujin::gui::comctl32 {

class LabelControl : public Window {
public:
	static const Size DefaultSize;

	LabelControl(const LayoutParam& lp = {});
	virtual ~LabelControl() = default;

	virtual CreateParam OnCreateParam() override;

private:
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(const LayoutParam& lp);
};

}

#endif
