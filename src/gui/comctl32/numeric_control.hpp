#ifndef SHOUJIN_SOURCE_GUI_COMCTL32_NUMERIC_CONTROL
#define SHOUJIN_SOURCE_GUI_COMCTL32_NUMERIC_CONTROL

#include "edit_control.hpp"
#include <optional>

namespace shoujin::gui::comctl32 {

class NumericControl : public EditControl {
public:
	NumericControl(LayoutParam const& = {});
	virtual ~NumericControl() = default;

	[[nodiscard]] std::optional<int> min_value() const { return _min_value; }
	[[nodiscard]] std::optional<int> max_value() const { return _max_value; }

	void min_value(int value) { _min_value = value; }
	void max_value(int value) { _max_value = value; }

	[[nodiscard]] int GetValue() const;
	int SetValue(int value) const;

protected:
	virtual bool OnChange() override;

private:
	virtual Window* Clone() const override;
	static LayoutParam BuildLayout(LayoutParam layout);
	static void OnLayoutReset(Layout& layout, void* userdata);
	int SetValue(int value, tstring const& previous_text) const;

	decltype(EditControl::OnLayoutResetEvent) _parent_onlayoutreset_event;
	int _onchange_depth{};
	std::optional<int> _min_value{};
	std::optional<int> _max_value{};
};

}

#endif
