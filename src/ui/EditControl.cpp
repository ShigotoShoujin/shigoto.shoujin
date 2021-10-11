#include "EditControl.hpp"

EditControl::EditControl(const Args& args) noexcept :
	Control{args, TEXT("EDIT")}
{
}

void EditControl::SetFocus() noexcept
{
	Control::SetFocus();
	auto text_length = SendMessage(GetHandle(), EM_LINELENGTH, 0, 0);
	SendMessage(GetHandle(), EM_SETSEL, 0, text_length);
}
