#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;

static bool OnCreatePostCloseMsg(const Window& window, const CREATESTRUCT& createparam, void* userdata);
static bool OnErrorOutput(tstring message, void* userdata);

TEST_CLASS(WindowTest) {
public:
	TEST_METHOD_INITIALIZE(TestInitialize)
	{
		shoujin::assert::OnErrorOutputEvent = OnErrorOutput;
		shoujin::gui::logging::_activate_wndproc_messagelog_ = true;
	}

	TEST_METHOD_CLEANUP(TestCleanup)
	{
		shoujin::assert::OnExitProcessEvent = nullptr;
		shoujin::gui::logging::_activate_wndproc_messagelog_ = false;
	}

	TEST_METHOD(Window_IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Window>);
	}

	TEST_METHOD(Window_IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Window>);
	}

	TEST_METHOD(Window_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Window>);
	}

	TEST_METHOD(Window_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Window>);
	}

	TEST_METHOD(Window_NewInstance_NoHandle) {
		Window window;
		auto handle = window.handle();
		Assert::IsNull(handle);
	}

	TEST_METHOD(Window_ShowModal_NoAssertions) {
		Window window{LayoutParam{.create_mode = LayoutMode::CenterParent}};
		window.OnCreateEvent = OnCreatePostCloseMsg;

		window.ShowModal();
	}

	TEST_METHOD(Window_ShowAndProcessMessageQueue_NoAssertions) {
		Window window{LayoutParam{.create_mode = LayoutMode::CenterParent}};

		window.Show();
		while(window.ProcessMessageQueue())
			PostMessage(*window.handle(), WM_CLOSE, 0, 0);
	}

	TEST_METHOD(Window_CopyConstructor_CopiedWithoutHandle) {
		Window window{};

		Window copied(window);

		Assert::IsNull(copied.handle());
	}

	TEST_METHOD(Window_AddChild_ShowModal_NoAssertions) {
		Window window{};
		window.OnCreateEvent = OnCreatePostCloseMsg;

		window.AddChild(new Window(LayoutParam{.window_size = window.window_size() / 2, .exstyle = WS_EX_STATICEDGE}));
		window.ShowModal();
	}

	TEST_METHOD(Window_AddChild_CopyConstructor_ShowModal_NoAssertions) {
		Window window{};
		window.AddChild(new Window(LayoutParam{.window_size = window.window_size() / 2, .exstyle = WS_EX_STATICEDGE}));
		Window copied(window);
		window.OnCreateEvent = OnCreatePostCloseMsg;
		copied.OnCreateEvent = OnCreatePostCloseMsg;

		window.ShowModal();
		copied.ShowModal();
	}

	TEST_METHOD(Window_AddEditControl_NoAssertions) {
		class EditControl : public Window {
			LayoutParam AdjustLayout(const LayoutParam& lp)
			{
				Size client_size;
				if(!lp.window_size && !lp.client_size)
					client_size = Size{256, 16};

				LayoutParam layout = lp;
				layout.client_size = client_size;
				layout.exstyle = WS_EX_CLIENTEDGE;

				return layout;
			}

		public:
			EditControl(const LayoutParam& lp = {}) :
				Window{AdjustLayout(lp)}
			{
			}

			virtual CreateParam OnCreateParam() override
			{
				return CreateParam{.classname = TEXT("EDIT"), .need_subclassing = true};
			}
		};

		Window window{};
		//window.OnCreateEvent = OnCreatePostCloseMsg;

		window.AddChild(new EditControl(LayoutParam{.position{11, 11}}));
		window.AddChild(new EditControl(LayoutParam{.position{11, 42}, .tabstop{false}}));
		window.AddChild(new EditControl(LayoutParam{.position{11, 73}}));
		window.AddChild(new EditControl(LayoutParam{.position{11, 106}}));
		window.AddChild(new EditControl(LayoutParam{.position{11, 139}}));

		window.ShowModal();
	}
};

static bool OnCreatePostCloseMsg(const Window& window, const CREATESTRUCT& createparam, void* userdata)
{
	SHOUJIN_ASSERT(window.handle());
	PostMessage(window.handle()->hwnd(), WM_CLOSE, 0, 0);
	return Window::kMsgNotHandled;
}

static bool OnErrorOutput(tstring message, void* userdata)
{
	Logger::WriteMessage(message.c_str());
	throw message;
}
