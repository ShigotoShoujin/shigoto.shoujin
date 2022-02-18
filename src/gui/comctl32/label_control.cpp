#include "label_control.hpp"

namespace shoujin::gui::comctl32 {

static void OnLayoutReset(Layout& layout, void* userdata);

Size const LabelControl::DefaultSize{80, 23};

LabelControl::LabelControl(LayoutParam const& layout_param) :
	Comctl32{TEXT("STATIC"), BuildLayout(layout_param)}
{
	OnLayoutResetEvent = {OnLayoutReset, this};
}

Window* LabelControl::Clone() const
{
	return new LabelControl(*this);
}

LayoutParam LabelControl::BuildLayout(LayoutParam const& layout_param)
{
	LayoutParam layout = layout_param;

	if(!layout_param.window_size && !layout_param.client_size)
		layout.window_size = DefaultSize;

	//layout.style = WS_BORDER;
	layout.tabstop = false;

	return layout;
}

static void OnLayoutReset(Layout& layout, void* userdata)
{
	layout.SetStyle(layout.style() | WS_BORDER, layout.exstyle());
	layout.SetTabStop(false);
}

}
