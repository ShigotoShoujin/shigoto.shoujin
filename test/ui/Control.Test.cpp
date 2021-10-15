#include <ui/ui.hpp>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ControlTest) {
public:
	TEST_METHOD(ControlTest_Test) {
		Window::WindowCreateInfo wci{
			.client_size = {640, 480},
			.layout = Window::Layout::CenterParent,
			.text = TEXT("ControlTest_Test")};

		Control wnd(wci);

		int y = 10;

		wnd.AddChild(EditControl{{10, y}, {200, 20}, TEXT("New Edit Control")});
		wnd.AddChild(EditControl{{10, y += 30}, {200, 20}, TEXT("New Edit Control")});
		wnd.AddChild(EditControl{{10, y += 30}, {200, 20}, TEXT("New Edit Control")});
		wnd.AddChild(EditControl{{10, y += 30}, {200, 20}, TEXT("New Edit Control")});
		wnd.AddChild(EditControl{{10, y += 30}, {200, 20}, TEXT("New Edit Control")});

		wnd.Show();
		wnd.MessageLoop();
	}
};
