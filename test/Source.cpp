#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Window/Window.hpp"
#include "Window/DialogWindow.hpp"
#include "Window/Control.hpp"
#include "Window/EditControl.hpp"

class UserControl : public Control {
};

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{
	DialogWindow dlg({.text = TEXT("Caption"), .window_size = {640, 480}});
	EditControl::Args edit_args = {.hwnd_parent = dlg.GetHandle(), .text = TEXT("Hello Edit"), .position{10, 10}, .size{120, 20}};

	for(int i = 0; i < 5; ++i) {
		dlg.AddOwnedControl(new EditControl(edit_args));
		edit_args.position.y += 30;
	}

	edit_args.position.x += 130;
	edit_args.position.y = 10;

	for(int i = 0; i < 5; ++i) {
		dlg.AddOwnedControl(new EditControl(edit_args));
		edit_args.position.y += 30;
	}

	dlg.Show();
	dlg.MessageLoop();

	return 0;
}
