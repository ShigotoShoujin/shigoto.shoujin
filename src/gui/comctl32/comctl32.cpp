#include "comctl32.hpp"

namespace shoujin::gui::comctl32 {

Comctl32::Comctl32(LPCTSTR classname, LayoutParam const& layout_param) :
	_classname{classname},
	Window(layout_param)
{}

void Comctl32::BeforeCreate(CreateParam& create_param)
{
	create_param.classname = _classname;
	create_param.subclass_window = true;
}

}
