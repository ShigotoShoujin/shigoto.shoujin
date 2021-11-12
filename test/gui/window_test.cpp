#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;
using namespace shoujin::gui::comctl32;

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
		shoujin::assert::OnErrorOutputEvent = nullptr;
		shoujin::gui::logging::_activate_wndproc_messagelog_ = false;
	}

	Window& CreateSampleWindow()
	{
		Window* window = new Window{LayoutParam{.layout_mode{LayoutMode::CenterParent}}};
		window->OnCreateEvent = OnCreatePostCloseMsg;

		int y = 11, r = 20 + 7;
		window->AddChild(new EditControl(LayoutParam{.position{11, y}}));
		window->AddChild(new EditControl(LayoutParam{.position{11, y += r}, .tabstop{false}}));
		window->AddChild(new EditControl(LayoutParam{.position{11, y += r}}));
		window->AddChild(new EditControl(LayoutParam{.position{11, y += r}}));
		window->AddChild(new EditControl(LayoutParam{.position{11, y += r}}));

		return *window;
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
		Window window{LayoutParam{.layout_mode = LayoutMode::CenterParent}};
		window.OnCreateEvent = OnCreatePostCloseMsg;

		window.ShowModal();
	}

	TEST_METHOD(Window_ProcessMessageQueue_NoAssertions) {
		Window window{LayoutParam{.layout_mode = LayoutMode::CenterParent}};

		window.Show();
		while(window.ProcessMessageQueue())
			PostMessage(*window.handle(), WM_CLOSE, 0, 0);
	}

	TEST_METHOD(Window_CopyConstructor_CopiedWithoutHandle) {
		Window window{};

		Window copied(window);

		Assert::IsNull(copied.handle());
	}

	TEST_METHOD(Window_AddChild_NoAssertions) {
		Window window{};
		window.OnCreateEvent = OnCreatePostCloseMsg;

		window.AddChild(new Window(LayoutParam{.window_size = window.window_size() / 2, .exstyle = WS_EX_STATICEDGE}));
		window.ShowModal();
	}

	TEST_METHOD(Window_CopyConstructor_CopiedProperly) {
		Window& window = CreateSampleWindow();

		Window copied(window);
		copied.OnCreateEvent = OnCreatePostCloseMsg;

		window.ShowModal();
		copied.ShowModal();
	}

	TEST_METHOD(Window_CopyAssignment_CopiedProperly) {
		Window& window = CreateSampleWindow();

		Window copied;
		copied = window;
		copied.OnCreateEvent = OnCreatePostCloseMsg;

		window.ShowModal();
		copied.ShowModal();
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
