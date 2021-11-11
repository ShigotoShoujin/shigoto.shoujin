#pragma once
#include "../window.hpp"

namespace shoujin::gui::comctl32 {

class EditControl : public Window {
	static const Size DefaultClientSize;

public:
	EditControl(const LayoutParam& lp = {});
	virtual ~EditControl() = default;

	virtual CreateParam OnCreateParam() override;

private:
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(const LayoutParam& lp);
};

}
