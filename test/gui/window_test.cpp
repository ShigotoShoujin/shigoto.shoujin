#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;
using namespace shoujin::gui::comctl32;

static bool OnCreatePostCloseMsg(Window const& window, CREATESTRUCT const& createparam, void* userdata);
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

	TEST_METHOD(IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Window>);
	}

	TEST_METHOD(IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Window>);
	}

	TEST_METHOD(IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Window>);
	}

	TEST_METHOD(IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Window>);
	}

	TEST_METHOD(NewInstance_NoHandle) {
		Window window;
		auto handle = window.handle();
		Assert::IsNull(handle);
	}

	TEST_METHOD(ShowModal_NoAssertions) {
		Window window{LayoutParam{.layout_mode = LayoutMode::CenterParent}};
		window.OnCreateEvent = OnCreatePostCloseMsg;

		window.ShowModal();
	}

	TEST_METHOD(ProcessMessageQueue_NoAssertions) {
		Window window{LayoutParam{.layout_mode = LayoutMode::CenterParent}};

		window.Show();
		while(window.ProcessMessageQueue())
			PostMessage(*window.handle(), WM_CLOSE, 0, 0);
	}

	TEST_METHOD(CopyConstructor_CopiedWithoutHandle) {
		Window window{};

		Window copied(window);

		Assert::IsNull(copied.handle());
	}

	TEST_METHOD(AddChild_NoAssertions) {
		Window window{};
		window.OnCreateEvent = OnCreatePostCloseMsg;

		window.AddChild(new Window(LayoutParam{.window_size = window.window_size() / 2, .exstyle = WS_EX_STATICEDGE}));
		window.ShowModal();
	}

	TEST_METHOD(CopyConstructor_CopiedProperly) {
		Window& window = CreateSampleWindow();

		Window copied(window);
		copied.OnCreateEvent = OnCreatePostCloseMsg;

		window.ShowModal();
		copied.ShowModal();
	}

	TEST_METHOD(CopyAssignment_CopiedProperly) {
		Window& window = CreateSampleWindow();

		Window copied;
		copied = window;
		copied.OnCreateEvent = OnCreatePostCloseMsg;

		window.ShowModal();
		copied.ShowModal();
	}
};

static bool OnCreatePostCloseMsg(Window const& window, CREATESTRUCT const& createparam, void* userdata)
{
	SHOUJIN_ASSERT(window.handle());
	PostMessage(window.handle()->hwnd(), WM_CLOSE, 0, 0);
	return false;
}

static bool OnErrorOutput(tstring message, void* userdata)
{
	Logger::WriteMessage(message.c_str());
	Assert::Fail(ToWideString(message).c_str());
}
