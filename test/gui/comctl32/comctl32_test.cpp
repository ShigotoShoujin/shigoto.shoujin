#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>

using namespace shoujin;
using namespace shoujin::gui;
using namespace shoujin::gui::comctl32;

static bool OnCreatePostCloseMsg(Window const& window, CREATESTRUCT const& createparam, void* userdata);
static bool OnErrorOutput(tstring message, void* userdata);

class TestControl : public Comctl32 {
public:
	TestControl(LayoutParam const& layout_param = {}) :
		Comctl32{TEXT("EDIT"), BuildLayout(layout_param)}
	{}

	virtual ~TestControl() = default;

	virtual void OnInitialize() override
	{
		++_on_initialize_call_count;
	}

	int OnInitializeCallCount() const { return _on_initialize_call_count; }

private:
	LayoutParam BuildLayout(LayoutParam layout_param)
	{
		layout_param.text = TEXT("Control Window");
		return layout_param;
	}

	virtual Window* Clone() const override { return new TestControl(*this); }
	int _on_initialize_call_count{};
};

class TestWindow : public Window {
public:
	explicit TestWindow(LayoutParam const& layout_param = {}) :
		Window(BuildLayout(layout_param))
	{
		_control = new TestControl(LayoutParam{.anchor = Anchor::AnchorTop | Anchor::AnchorLeft, .window_size{200, 23}, .exstyle{WS_EX_CLIENTEDGE}});
		AddChild(_control);
	}

	virtual ~TestWindow() = default;

	TestControl& EditControl() { return *_control; }

private:
	LayoutParam BuildLayout(LayoutParam layout_param)
	{
		layout_param.text = TEXT("Main Window");
		return layout_param;
	}

	TestControl* _control;
};

TEST_CLASS(Comctl32Test) {
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

	TEST_METHOD(GivenAWindowWithAControl_WhenWindowIsCreated_ControlOnInitializeIsCalledOnce) {
		TestWindow window{LayoutParam{.layout_mode = LayoutMode::CenterParent}};
		window.OnCreateEvent = OnCreatePostCloseMsg;

		window.ShowModal();

		Assert::AreEqual(1, window.EditControl().OnInitializeCallCount());
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
