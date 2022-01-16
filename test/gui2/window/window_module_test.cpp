import Shoujin.Gui.Window;

#include "CppUnitTest.h"
#include "test_tools.hpp"
//#include <shoujin/tstring.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace shoujin::gui2;
using namespace shoujin::test::gui2::test_tools;

namespace shoujin::test::gui2 {

TEST_CLASS(WindowTest) {
public:
	TEST_METHOD_INITIALIZE(TestInitialize)
	{
	}

	TEST_METHOD_CLEANUP(TestCleanup)
	{
	}

	TEST_METHOD(GivenWindowCreatedWithDefaultConstructor_WhenShowModalIsCalled_ShowCenteredWithNiceDefault) {
		Window w;
		
		//w.ShowModal();

		//Assert::AreEqual(L"Shoujin", w.text());
	}
};

}
