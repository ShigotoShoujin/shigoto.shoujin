#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/gui.hpp>
#include <shoujin/tstring.hpp>
#include <format>

using namespace shoujin;
using namespace shoujin::gui;

TEST_CLASS(WindowMouseTest) {
public:
	BEGIN_TEST_METHOD_ATTRIBUTE(ShowWindow_LogEvents)
	TEST_IGNORE()
	END_TEST_METHOD_ATTRIBUTE()
	TEST_METHOD(ShowWindow_LogEvents) {
		Window* window = new Window{LayoutParam{.layout_mode{LayoutMode::CenterParent}, .client_size{320, 640}, .text = TEXT("Mouse Click")}};

		EditControl* edit = new EditControl(LayoutParam{
			.anchor{Anchor::Top | Anchor::Left},
			.client_size{180, 500},
			.style = ES_MULTILINE});

		window->AddChild(edit);

		struct UserData {
			EditControl* edit;
			tstring_view text;
		};

		UserData wnd_mousedown{edit, L"Window Down"};
		UserData wnd_mouseup{edit, L"Window Up"};
		UserData wnd_mouseclick{edit, L"Window Click"};
		UserData edt_mousedown{edit, L"Edit Down"};
		UserData edt_mouseup{edit, L"Edit Up"};
		UserData edt_mouseclick{edit, L"Edit Click"};

		auto mouseHandler = [](Window*, Window::MouseEvent const&, void* userdata) -> bool {
			auto ud = static_cast<UserData*>(userdata);
			ud->edit->AppendLine(ud->text);
			return Window::Handled;
		};

		window->OnMouseDownEvent = {mouseHandler, &wnd_mousedown};
		window->OnMouseUpEvent = {mouseHandler, &wnd_mouseup};
		window->OnMouseClickEvent = {mouseHandler, &wnd_mouseclick};
		edit->OnMouseDownEvent = {mouseHandler, &edt_mousedown};
		edit->OnMouseUpEvent = {mouseHandler, &edt_mouseup};
		edit->OnMouseClickEvent = {mouseHandler, &edt_mouseclick};

		window->ShowModal();
	}
};
