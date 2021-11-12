#pragma once
#include "../window.hpp"

namespace shoujin::gui::comctl32 {

class ColorControl : public Window {
	static const Size DefaultClientSize;

public:
	ColorControl(const LayoutParam& lp = {});
	virtual ~ColorControl() = default;

	virtual CreateParam OnCreateParam() override;

private:
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(const LayoutParam& lp);
};

}
