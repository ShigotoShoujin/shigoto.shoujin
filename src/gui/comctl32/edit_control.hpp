#ifndef SHOUJIN_SOURCE_GUI_COMCTL32_EDIT_CONTROL
#define SHOUJIN_SOURCE_GUI_COMCTL32_EDIT_CONTROL

#include "comctl32.hpp"

namespace shoujin::gui::comctl32 {

class EditControl : public Comctl32 {
public:
	static Size const DefaultSize;

	EditControl(LayoutParam const& = {});
	virtual ~EditControl() = default;

	[[nodiscard]] virtual bool autoselect() const;
	[[nodiscard]] virtual bool readonly() const;

	virtual void autoselect(bool value);
	virtual void readonly(bool value);

	Event<bool, EditControl*> OnChangeEvent;

protected:
	virtual void OnInitialize(Window* source) override;
	virtual bool OnCommand(int notification_code) override;
	virtual bool OnChange();
	virtual bool OnKeyPress(KeyEvent const& e) override;

private:
	virtual MessageResult RaiseOnChange();

	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(LayoutParam layout);
	bool _autoselect{};
	bool _readonly{};
};

}

#endif
