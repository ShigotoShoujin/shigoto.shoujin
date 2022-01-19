import Shoujin.Gui.Layout;
import Shoujin.Gui.Window;

#include "CppUnitTest.h"
//#include <shoujin/tstring.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace shoujin::gui2;
using namespace shoujin::gui2::layout;

namespace shoujin::test::gui2 {

TEST_CLASS(WindowTest) {
public:
	TEST_METHOD_INITIALIZE(TestInitialize)
	{
	}

	TEST_METHOD_CLEANUP(TestCleanup)
	{
	}

	TEST_METHOD(GivenDefaultConstructor_WhenShown_IsCentered) {
		Window w;
		
		w.showModal();

		//w.client
		//Assert::AreEqual(L"Shoujin", w.text());
	}
};

}
