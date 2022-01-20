import Shoujin.Gui.Window;

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace shoujin::gui2;
using namespace shoujin::gui2::layout;

namespace shoujin::test::gui2 {

TEST_CLASS(WindowTest) {
public:
	TEST_METHOD_INITIALIZE(testInitialize)
	{
	}

	TEST_METHOD_CLEANUP(testCleanup)
	{
	}

	TEST_METHOD(givenDefaultConstructor_WhenShown_IsCentered) {
		Window w;
		
		w.showModal();

		//w.client
		//Assert::AreEqual(L"Shoujin", w.text());
	}
};

}
