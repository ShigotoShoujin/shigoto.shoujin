#pragma once
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
