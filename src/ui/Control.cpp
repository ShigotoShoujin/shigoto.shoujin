#include "Control.hpp"

const int Control::AUTO = -1;

static Window::Args MakeArgs(const Control::Args& args, LPCTSTR classname) noexcept
{
	Window::Args window_args{
		.hwnd_parent = args.hwnd_parent,
		.classname = classname,
		.text = args.text,
		.mode = args.mode,
		.position = args.position,
		.window_size = args.size,
		.style = args.style | WS_VISIBLE,
		.ex_style = args.style | WS_EX_STATICEDGE,
		.hwnd_menu = (HMENU)args.id,
	};

	return window_args;
}

Control::Control(Args args, LPCTSTR classname) noexcept :
	Window{MakeArgs(args, classname)},
	id{args.id},
	taborder{args.taborder},
	tabstop{args.tabstop}
{
}

void Control::SetFocus() noexcept
{
	::SetFocus(GetHandle());
}
