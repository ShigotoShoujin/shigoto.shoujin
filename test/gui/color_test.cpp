#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/gui.hpp>
#include <cmath>

using namespace shoujin::gui;

namespace Microsoft::VisualStudio::CppUnitTestFramework {

template<> inline std::wstring ToString<Color>(Color const& t)
{
	std::wstringstream wss;
	ColorByteRGB cb = t;
	wss << L"RGB(" << cb.R << L',' << cb.G << L',' << cb.B << L')';
	return wss.str();
}

template<> inline std::wstring ToString<ColorHSL>(ColorHSL const& t)
{
	std::wstringstream wss;
	wss << L"HSL(" << t.H << L',' << t.S * 100 << L"%," << t.L * 100 << L"%)";
	return wss.str();
}

}

bool operator==(ColorHSL const& lhs, ColorHSL const& rhs)
{
	return (
		std::roundf(lhs.H) == std::roundf(rhs.H) &&
		std::roundf(lhs.S * 100) == std::roundf(rhs.S * 100) &&
		std::roundf(lhs.L * 100) == std::roundf(rhs.L * 100));
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
		Assert::AreEqual<Color>(c, RGB(128, 0, 255));
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

	TEST_METHOD(ConvertToColorHSL_WhenBlack_HSLZero) {
		Color c = 0;
		ColorHSL expected{0.f, 0.f, 0.f};

		ColorHSL hsl = c;

		Assert::AreEqual(expected, hsl);
	}

	TEST_METHOD(ConvertToColorHSL_WhenRed_H0S100L50) {
		Color c = ColorByteRGB(255, 0, 0);
		ColorHSL expected{0.f, 1.f, .5f};

		ColorHSL hsl = c;

		Assert::AreEqual(expected, hsl);
	}

	TEST_METHOD(ConvertToColorHSL_WhenGreen_H120S100L50) {
		Color c = ColorByteRGB(0, 255, 0);
		ColorHSL expected{120.f, 1.f, .5f};

		ColorHSL hsl = c;

		Assert::AreEqual(expected, hsl);
	}

	TEST_METHOD(ConvertToColorHSL_WhenBlue_H240S100L50) {
		Color c = ColorByteRGB(0, 0, 255);
		ColorHSL expected{240.f, 1.f, .5f};

		ColorHSL hsl = c;

		Assert::AreEqual(expected, hsl);
	}

	TEST_METHOD(ConvertToColorHSL_WhenR91G48B156_H264S53L40) {
		Color c = ColorByteRGB(91, 48, 156);
		ColorHSL expected{264.f, .53f, .40f};

		ColorHSL hsl = c;

		Assert::AreEqual(expected, hsl);
	}

	TEST_METHOD(ConvertToColorHSL_WhenR127G127B127_H0S0L50) {
		Color c = ColorByteRGB(127, 127, 127);
		ColorHSL expected{0.f, 0.f, .5f};

		ColorHSL hsl = c;

		Assert::AreEqual(expected, hsl);
	}

	TEST_METHOD(ConvertFromColorHSL_WhenHSLZero_Black) {
		ColorHSL hsl{};
		Color expected;

		Color color = hsl;

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertFromColorHSL_WhenH0S100L50_Red) {
		ColorHSL hsl{0.f, 1.f, .5f};
		Color expected{ColorByteRGB{255, 0, 0}};

		Color color = hsl;

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertFromColorHSL_WhenH120S100L50_Green) {
		ColorHSL hsl{120.f, 1.f, .5f};
		Color expected{ColorByteRGB{0, 255, 0}};

		Color color = hsl;

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertFromColorHSL_WhenH240S100L50_Blue) {
		ColorHSL hsl{240.f, 1.f, .5f};
		Color expected{ColorByteRGB{0, 0, 255}};

		Color color = hsl;

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertFromColorHSL_WhenH264S53L40_R91G48B156) {
		ColorHSL hsl{264.f, .53f, .4f};
		Color expected{ColorByteRGB{91, 48, 156}};

		Color color = hsl;

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertFromColorHSL_WhenH0S0L50_R128G128B128) {
		ColorHSL hsl{0.f, 0.f, .5f};
		Color expected{ColorByteRGB{128, 128, 128}};

		Color color = hsl;

		Assert::AreEqual(expected, color);
	}
};
