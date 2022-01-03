#ifndef SHOUJIN_SOURCE_GUI_COMCTL32_EDIT_CONTROL
#define SHOUJIN_SOURCE_GUI_COMCTL32_EDIT_CONTROL

#include "comctl32.hpp"

namespace shoujin::gui::comctl32 {

class EditControl : public Comctl32 {
public:
	static Size const DefaultSize;

	EditControl(LayoutParam const& = {});
	virtual ~EditControl() = default;

	[[nodiscard]] virtual bool selectall_on_focus() const;
	virtual void selectall_on_focus(bool value);

	virtual bool OnCommand(int notification_code) override;

private:
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(LayoutParam layout);
	bool _selectall_on_focus{};
};

}

#endif
