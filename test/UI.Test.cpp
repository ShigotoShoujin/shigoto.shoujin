#include "CppUnitTest.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <ui/Window.hpp>
#include <ui/DialogWindow.hpp>
#include <ui/ButtonControl.hpp>
#include <ui/EditControl.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(UI) {
public:
	TEST_METHOD(DialogCycleTab) {
		const int BUTTON_OK_ID = 1;

		struct DialogTest : public DialogWindow {
			DialogTest() :
				DialogWindow{{.text = TEXT("Caption"), .client_size = {640, 480}}}
			{
				EditControl::Args edit_args = {{.hwnd_parent = GetHandle(), .text = TEXT("Hello Edit"), .position{10, 10}, .size{120, 20}}};

				for(int i = 0; i < 5; ++i) {
					AddOwnedControl(new EditControl(edit_args));
					edit_args.position.y += 30;
				}

				edit_args.position.x += 130;
				edit_args.position.y = 10;

				for(int i = 0; i < 5; ++i) {
					AddOwnedControl(new EditControl(edit_args));
					edit_args.position.y += 30;
				}

				edit_args.size = {256, 64};
				edit_args.position = {374, 10};
				edit_args.style = ES_MULTILINE | ES_READONLY;
				edit_args.text = TEXT("Try using TAB and SHIFT TAB to move between these controls.\r\nOK or Escape to close.");
				AddOwnedControl(new EditControl(edit_args));

				ButtonControl::Args button_args = {{.id = BUTTON_OK_ID, .hwnd_parent = GetHandle(), .text = TEXT("OK"), .position{534, 446}, .size{96, 24}}};
				AddOwnedControl(new ButtonControl(button_args));
			}

			LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) noexcept override
			{
				switch(msg) {
					case WM_COMMAND:
						switch(LOWORD(wparam)) {
							case BUTTON_OK_ID:
								this->Destroy();
								return 0;
						}
				}

				return Window::WndProc(msg, wparam, lparam);
			}
		} dlg;

		dlg.Show();
		dlg.MessageLoop();
	}
};
