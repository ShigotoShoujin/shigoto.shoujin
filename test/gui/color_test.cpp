#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/gui.hpp>
#include <cmath>

using namespace shoujin::gui;

namespace Microsoft::VisualStudio::CppUnitTestFramework {

template<> inline std::wstring ToString<Color>(Color const& t)
{
	std::wstringstream wss;
	ColorByteRGB cbrgb = t;
	wss << L"RGB(" << cbrgb.R << L',' << cbrgb.G << L',' << cbrgb.B << L')';
	return wss.str();
}

template<> inline std::wstring ToString<ColorFloatHSL>(ColorFloatHSL const& t)
{
	std::wstringstream wss;
	wss << L"HSL(" << t.H << L',' << t.S * 100 << L"%," << t.L * 100 << L"%)";
	return wss.str();
}

}

bool operator==(ColorFloatHSL const& lhs, ColorFloatHSL const& rhs)
{
	return (
		std::roundf(lhs.H) == std::roundf(rhs.H) &&
		std::roundf(lhs.S * 100) == std::roundf(rhs.S * 100) &&
		std::roundf(lhs.L * 100) == std::roundf(rhs.L * 100));
}

TEST_CLASS(ColorTest) {
public:
	TEST_METHOD(ConstructorDefault_Black) {
		Color color;
		Assert::AreEqual(color, Color::Black);
	}

	TEST_METHOD(ConstructorColorByteRGB_AssignColor) {
		ColorByteRGB cbrgb{43, 34, 122};
		Color color(cbrgb);
		Assert::AreEqual<Color>(color, RGB(43, 34, 122));
	}

	TEST_METHOD(ConstructorColorByteFloat_AssignColor) {
		ColorFloatRGB cfrgb{.5f, 0.f, 1.f};
		Color c(cfrgb);
		Assert::AreEqual<Color>(c, RGB(128, 0, 255));
	}

	TEST_METHOD(ConvertToColorByteRGB_MatchOriginal) {
		auto color = Color::Navy;
		ColorByteRGB cbrgb = color;
		Assert::AreEqual<Color>(color, cbrgb);
	}

	TEST_METHOD(ConvertToColorFloatRGB_MatchOriginal) {
		auto color = Color::Navy;
		ColorFloatRGB cfrgb = color;
		Assert::AreEqual<Color>(color, cfrgb);
	}

	TEST_METHOD(ConvertToColorFloatHSL_WhenBlack_HSLZero) {
		Color color = 0;
		ColorFloatHSL expected{0.f, 0.f, 0.f};

		ColorFloatHSL cfhsl = color;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertToColorFloatHSL_WhenRed_H0S100L50) {
		Color color = ColorByteRGB(255, 0, 0);
		ColorFloatHSL expected{0.f, 1.f, .5f};

		ColorFloatHSL cfhsl = color;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertToColorFloatHSL_WhenGreen_H120S100L50) {
		Color color = ColorByteRGB(0, 255, 0);
		ColorFloatHSL expected{120.f, 1.f, .5f};

		ColorFloatHSL cfhsl = color;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertToColorFloatHSL_WhenBlue_H240S100L50) {
		Color color = ColorByteRGB(0, 0, 255);
		ColorFloatHSL expected{240.f, 1.f, .5f};

		ColorFloatHSL cfhsl = color;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertToColorFloatHSL_WhenWhite_H0S0L100) {
		Color color = ColorByteRGB(255, 255, 255);
		ColorFloatHSL expected{.0f, .0f, 1.f};

		ColorFloatHSL cfhsl = color;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertToColorFloatHSL_WhenR91G48B156_H264S53L40) {
		Color color = ColorByteRGB(91, 48, 156);
		ColorFloatHSL expected{264.f, .53f, .40f};

		ColorFloatHSL cfhsl = color;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertToColorFloatHSL_WhenR255G254B255_H300S100L100) {
		Color color = ColorByteRGB(255, 254, 255);
		ColorFloatHSL expected{300.f, 1.f, 1.f};

		ColorFloatHSL cfhsl = color;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertToColorFloatHSL_WhenR127G127B127_H0S0L50) {
		Color color = ColorByteRGB(127, 127, 127);
		ColorFloatHSL expected{0.f, 0.f, .5f};

		ColorFloatHSL cfhsl = color;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertFromColorFloatHSL_WhenHSLZero_Black) {
		ColorFloatHSL cfhsl{};
		Color expected;

		Color color = cfhsl;

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertFromColorFloatHSL_WhenH0S100L50_Red) {
		ColorFloatHSL cfhsl{0.f, 1.f, .5f};
		Color expected{ColorByteRGB{255, 0, 0}};

		Color color = cfhsl;

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertFromColorFloatHSL_WhenH120S100L50_Green) {
		ColorFloatHSL cfhsl{120.f, 1.f, .5f};
		Color expected{ColorByteRGB{0, 255, 0}};

		Color color = cfhsl;

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertFromColorFloatHSL_WhenH240S100L50_Blue) {
		ColorFloatHSL cfhsl{240.f, 1.f, .5f};
		Color expected{ColorByteRGB{0, 0, 255}};

		Color color = cfhsl;

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertFromColorFloatHSL_WhenH264S53L40_R91G48B156) {
		ColorFloatHSL cfhsl{264.f, .53f, .4f};
		Color expected{ColorByteRGB{91, 48, 156}};

		Color color = cfhsl;

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertFromColorFloatHSL_WhenH0S0L50_R128G128B128) {
		ColorFloatHSL cfhsl{0.f, 0.f, .5f};
		Color expected{ColorByteRGB{128, 128, 128}};

		Color color = cfhsl;

		Assert::AreEqual(expected, color);
	}
};
