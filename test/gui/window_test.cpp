#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;

static void OnCreateSendCloseMsg(const Window& window, const CREATESTRUCT& createparam, void* userdata);
static void OnErrorOutput(tstring message, bool& cancel, void* userdata);

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
		window.OnCreateEvent = OnCreateSendCloseMsg;

		window.ShowModal();
	}

	TEST_METHOD(Window_ShowAndProcessMessageQueue_NoAssertions) {
		Window window{LayoutParam{.create_mode = LayoutMode::CenterParent}};

		window.Show();
		while(window.ProcessMessageQueue())
			PostMessage(*window.handle(), WM_CLOSE, 0, 0);
	}
};

static void OnCreateSendCloseMsg(const Window& window, const CREATESTRUCT& createparam, void* userdata)
{
	SHOUJIN_ASSERT(window.handle());
	PostMessage(window.handle()->hwnd(), WM_CLOSE, 0, 0);
}

static void OnErrorOutput(tstring message, bool& cancel, void* userdata)
{
	Logger::WriteMessage(message.c_str());
	throw message;
}
