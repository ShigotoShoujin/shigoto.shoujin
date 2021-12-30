#ifndef SHOUJIN_SOURCE_GUI_COMCTL32_LABEL_CONTROL
#define SHOUJIN_SOURCE_GUI_COMCTL32_LABEL_CONTROL

#include "../window.hpp"

namespace shoujin::gui::comctl32 {

class LabelControl : public Window {
public:
	static Size const DefaultSize;

	LabelControl(LayoutParam const& lp = {});
	virtual ~LabelControl() = default;

	virtual CreateParam OnCreateParam() override;
	virtual void SetLayout(Layout const& layout) override;

private:
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(const LayoutParam& lp);
};

}

#endif
