#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/gui.hpp>

using namespace shoujin::gui;

namespace Microsoft::VisualStudio::CppUnitTestFramework {

template<> inline std::wstring ToString<Color>(Color const& t)
{
	std::wstringstream wss;
	ColorByteRGB cb = t;
	wss << L"(" << cb.R << L',' << cb.G << L',' << cb.B << L')';
	return wss.str();
}

}

TEST_CLASS(ColorTest) {
public:
	TEST_METHOD(ConstructorDefault_Black) {
		Color c;
		Assert::AreEqual(c, Color::Black);
	}

	TEST_METHOD(ConstructorColorByteRGB_AssignColor) {
		ColorByteRGB cb{43, 34, 122};
		Color c(cb);
		Assert::AreEqual<Color>(c, RGB(43, 34, 122));
	}

	TEST_METHOD(ConstructorColorByteFloat_AssignColor) {
		ColorFloatRGB cf{.5f, 0.f, 1.f};
		Color c(cf);
		Assert::AreEqual<Color>(c, RGB(127, 0, 255));
	}

	TEST_METHOD(ConvertToColorByteRGB_MatchOriginal) {
		auto c = Color::Navy;
		ColorByteRGB cb = c;
		Assert::AreEqual<Color>(c, cb);
	}

	TEST_METHOD(ConvertToColorFloatRGB_MatchOriginal) {
		auto c = Color::Navy;
		ColorFloatRGB cf = c;
		Assert::AreEqual<Color>(c, cf);
	}
};
