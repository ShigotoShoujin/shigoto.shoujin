#ifndef SHOUJIN_SOURCE_GUI_COMCTL32_EDIT_CONTROL
#define SHOUJIN_SOURCE_GUI_COMCTL32_EDIT_CONTROL

#pragma once
#include "../window.hpp"

namespace shoujin::gui::comctl32 {

class EditControl : public Window {
public:
	static Size const DefaultSize;

	EditControl(LayoutParam const& lp = {});
	virtual ~EditControl() = default;

	virtual CreateParam OnCreateParam() override;

private:
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(LayoutParam const& lp);
};

}

#endif
