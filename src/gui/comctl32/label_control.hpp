#ifndef SHOUJIN_SOURCE_GUI_COMCTL32_LABEL_CONTROL
#define SHOUJIN_SOURCE_GUI_COMCTL32_LABEL_CONTROL

#include "comctl32.hpp"

namespace shoujin::gui::comctl32 {

class LabelControl : public Comctl32 {
public:
	static Size const DefaultSize;

	LabelControl(LayoutParam const& = {});
	virtual ~LabelControl() = default;

private:
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(const LayoutParam&);
};

}

#endif
