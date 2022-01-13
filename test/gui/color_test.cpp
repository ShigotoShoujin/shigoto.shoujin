#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <cmath>
#include <shoujin/gui.hpp>

using namespace shoujin::gui;

namespace Microsoft::VisualStudio::CppUnitTestFramework {

template<> inline std::wstring ToString<Color>(Color const& t)
{
	std::wstringstream wss;
	ColorByteRGB cbrgb = t;
	wss << L"RGB(" << cbrgb.red << L',' << cbrgb.green << L',' << cbrgb.blue << L')';
	return wss.str();
}

template<> inline std::wstring ToString<ColorByteRGB>(ColorByteRGB const& t)
{
	std::wstringstream wss;
	wss << L"RGB(" << t.red << L',' << t.green << L',' << t.blue << L')';
	return wss.str();
}

template<> inline std::wstring ToString<ColorByteHSL>(ColorByteHSL const& t)
{
	std::wstringstream wss;
	wss << L"HSL(" << t.hue << L',' << t.saturation << L',' << t.lightness << L')';
	return wss.str();
}

template<> inline std::wstring ToString<ColorByteHSV>(ColorByteHSV const& t)
{
	std::wstringstream wss;
	wss << L"HSV(" << t.hue << L',' << t.saturation << L',' << t.value << L')';
	return wss.str();
}

template<> inline std::wstring ToString<ColorFloatRGB>(ColorFloatRGB const& t)
{
	std::wstringstream wss;
	wss << L"RGB(" << t.red << L',' << t.green << L"," << t.blue << L')';
	return wss.str();
}

template<> inline std::wstring ToString<ColorFloatHSL>(ColorFloatHSL const& t)
{
	std::wstringstream wss;
	wss << L"HSL(" << t.hue << L',' << t.saturation * 100 << L"%," << t.lightness * 100 << L"%)";
	return wss.str();
}

template<> inline std::wstring ToString<ColorFloatHSV>(ColorFloatHSV const& t)
{
	std::wstringstream wss;
	wss << L"HSV(" << t.hue << L',' << t.saturation * 100 << L"%," << t.value * 100 << L"%)";
	return wss.str();
}

}

TEST_CLASS(ColorTest) {
	float precision = 0.01f;

public:
	TEST_METHOD(ConvertHslToRgb_WhenH0S0L50_R128G128B128) {
		ColorFloatHSL cfhsl{0.f, 0.f, .5f};
		ColorByteRGB expected{128, 128, 128};

		ColorByteRGB color = Color(cfhsl);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHslToRgb_WhenH0S100L50_Red) {
		ColorFloatHSL cfhsl{0.f, 1.f, .5f};
		ColorByteRGB expected{255, 0, 0};

		ColorByteRGB color = Color(cfhsl);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHslToRgb_WhenH120S100L50_Green) {
		ColorFloatHSL cfhsl{120.f, 1.f, .5f};
		ColorByteRGB expected{0, 255, 0};

		ColorByteRGB color = Color(cfhsl);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHslToRgb_WhenH240S100L50_Blue) {
		ColorFloatHSL cfhsl{240.f, 1.f, .5f};
		ColorByteRGB expected{0, 0, 255};

		ColorByteRGB color = Color(cfhsl);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHslToRgb_WhenH264S53L40_R91G48B156) {
		ColorFloatHSL cfhsl{264.f, .53f, .4f};
		ColorByteRGB expected{91, 48, 156};

		ColorByteRGB color = Color{cfhsl};

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHslToRgb_WhenH360S100L50_R255G0B0) {
		ColorFloatHSL cfhsl{360.f, 1.f, .5f};
		ColorByteRGB expected{255, 0, 0};

		ColorByteRGB color = Color(cfhsl);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHslToRgb_WhenHSLZero_Black) {
		ColorFloatHSL cfhsl;
		Color expected;

		Color color = cfhsl;

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHsvToRgb_WhenH0S0L50_R128G128B128) {
		ColorFloatHSV cfhsv{0.f, 0.f, .502f};
		ColorByteRGB expected{128, 128, 128};

		ColorByteRGB color = Color(cfhsv);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHsvToRgb_WhenH0S100L50_Red) {
		ColorFloatHSV cfhsv{0.f, 1.f, 1.f};
		ColorByteRGB expected{255, 0, 0};

		ColorByteRGB color = Color(cfhsv);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHsvToRgb_WhenH120S100L50_Green) {
		ColorFloatHSV cfhsv{120.f, 1.f, 1.0f};
		ColorByteRGB expected{0, 255, 0};

		ColorByteRGB color = Color(cfhsv);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHsvToRgb_WhenH240S100L50_Blue) {
		ColorFloatHSV cfhsv{240.f, 1.f, 1.f};
		ColorByteRGB expected{0, 0, 255};

		ColorByteRGB color = Color(cfhsv);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHsvToRgb_WhenH264S53L40_R91G48B156) {
		ColorFloatHSV cfhsv{264.f, .692f, .612f};
		ColorByteRGB expected{91, 48, 156};

		ColorByteRGB color = Color(cfhsv);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHsvToRgb_WhenH360S100L50_R255G0B0) {
		ColorFloatHSV cfhsv{360.f, 1.f, 1.f};
		ColorByteRGB expected{255, 0, 0};

		ColorByteRGB color = Color(cfhsv);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertHsvToRgb_WhenHSVZero_Black) {
		ColorFloatHSV cfhsv;
		ColorByteRGB expected;

		ColorByteRGB color = Color(cfhsv);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenBlack_HSLZero) {
		Color color;
		ColorByteHSL expected;

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenBlue_H240S100L50) {
		Color color = ColorByteRGB(0, 0, 255);
		ColorByteHSL expected{240, 100, 50};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenGray_H0S0L50) {
		Color color = ColorByteRGB(128, 128, 128);
		ColorByteHSL expected{0, 0, 50};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenGreen_H120S100L50) {
		Color color = ColorByteRGB(0, 255, 0);
		ColorByteHSL expected{120, 100, 50};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenR255G254B255_H300S100L100) {
		Color color = ColorByteRGB(255, 254, 255);
		ColorByteHSL expected{300, 100, 100};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenR91G48B156_H264S53L40) {
		Color color = ColorByteRGB(91, 48, 156);
		ColorByteHSL expected{264, 53, 40};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenRed_H0S100L50) {
		Color color = ColorByteRGB(255, 0, 0);
		ColorByteHSL expected{0, 100, 50};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenWhite_H0S0L100) {
		Color color = ColorByteRGB(255, 255, 255);
		ColorByteHSL expected{0, 0, 100};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenBlack_HSVZero) {
		Color color;
		ColorByteHSV expected;

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenBlue_H240S100L50) {
		Color color = ColorByteRGB(0, 0, 255);
		ColorByteHSV expected{240, 100, 100};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenGray_H0S0L50) {
		Color color = ColorByteRGB(128, 128, 128);
		ColorByteHSV expected{0, 0, 50};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenGreen_H120S100L50) {
		Color color = ColorByteRGB(0, 255, 0);
		ColorByteHSV expected{120, 100, 100};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenR255G254B255_H300S100L100) {
		Color color = ColorByteRGB(255, 254, 255);
		ColorByteHSV expected{300, 0, 100};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenR91G48B156_H264S53L40) {
		Color color = ColorByteRGB(91, 48, 156);
		ColorByteHSV expected{264, 69, 61};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenRed_H0S100L50) {
		Color color = ColorByteRGB(255, 0, 0);
		ColorByteHSV expected{0, 100, 100};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenWhite_H0S0L100) {
		Color color = ColorByteRGB(255, 255, 255);
		ColorByteHSV expected{0, 0, 100};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(OperatorConvertToColorByteHSL_ColorMatch) {
		const auto red = 128;
		const auto green = 128;
		const auto blue = 128;

		ColorByteHSL actual = Color(ColorByteRGB(red, green, blue));

		Assert::AreEqual<int>(0, actual.hue);
		Assert::AreEqual<int>(0, actual.saturation);
		Assert::AreEqual<int>(50, actual.lightness);
	}

	TEST_METHOD(OperatorConvertToColorByteHSV_ColorMatch) {
		const auto red = 128;
		const auto green = 128;
		const auto blue = 128;

		ColorByteHSV actual = Color(ColorByteRGB(red, green, blue));

		Assert::AreEqual<int>(0, actual.hue);
		Assert::AreEqual<int>(0, actual.saturation);
		Assert::AreEqual<int>(50, actual.value);
	}

	TEST_METHOD(OperatorConvertToColorByteRGB_ColorMatch) {
		const auto red = 245;
		const auto green = 39;
		const auto blue = 167;

		ColorByteRGB actual = Color(ColorByteRGB(red, green, blue));

		Assert::AreEqual<int>(red, actual.red);
		Assert::AreEqual<int>(green, actual.green);
		Assert::AreEqual<int>(blue, actual.blue);
	}

	TEST_METHOD(OperatorConvertToColorFloatHSL_ColorMatch) {
		const auto red = 128;
		const auto green = 128;
		const auto blue = 128;

		ColorFloatHSL actual = Color(ColorByteRGB(red, green, blue));

		Assert::AreEqual(.0f, actual.hue);
		Assert::AreEqual(.0f, actual.saturation, .01f);
		Assert::AreEqual(.5f, actual.lightness, .01f);
	}

	TEST_METHOD(OperatorConvertToColorFloatHSV_ColorMatch) {
		const auto red = 128;
		const auto green = 128;
		const auto blue = 128;

		ColorFloatHSV actual = Color(ColorByteRGB(red, green, blue));

		Assert::AreEqual(.0f, actual.hue);
		Assert::AreEqual(.0f, actual.saturation, .01f);
		Assert::AreEqual(.5f, actual.value, .01f);
	}

	TEST_METHOD(OperatorConvertToColorFloatRGB_ColorMatch) {
		const auto red = 245;
		const auto green = 39;
		const auto blue = 167;

		ColorFloatRGB actual = Color(ColorByteRGB(red, green, blue));

		Assert::AreEqual(red / 255.f, actual.red);
		Assert::AreEqual(green / 255.f, actual.green);
		Assert::AreEqual(blue / 255.f, actual.blue);
	}

	TEST_METHOD(OperatorConvertToCOLORREF_ColorMatch) {
		const auto red = 245;
		const auto green = 39;
		const auto blue = 167;

		COLORREF actual = Color(ColorByteRGB(red, green, blue));

		Assert::AreEqual<int>(red, GetRValue(actual));
		Assert::AreEqual<int>(green, GetGValue(actual));
		Assert::AreEqual<int>(blue, GetBValue(actual));
	}

	TEST_METHOD(ConvertHslToHsv_WhenH0S0L50_H0S0V50) {
		ColorFloatHSL cfhsl{0.f, 0.f, .5f};
		ColorFloatHSV expected{0.f, 0.f, .5f};

		ColorFloatHSV cfhsv = cfhsl;

		Assert::AreEqual(expected, cfhsv);
	}

	TEST_METHOD(ConvertHslToHsv_WhenH0S100L50_Red) {
		ColorFloatHSL cfhsl{0.f, 1.f, .5f};
		ColorFloatHSV expected{0.f, 1.f, 1.f};

		ColorFloatHSV cfhsv = cfhsl;

		Assert::AreEqual(expected, cfhsv);
	}

	TEST_METHOD(ConvertHslToHsv_WhenH120S100L50_Green) {
		ColorFloatHSL cfhsl{120.f, 1.f, .5f};
		ColorFloatHSV expected{120.f, 1.f, 1.f};

		ColorFloatHSV cfhsv = cfhsl;

		Assert::AreEqual(expected, cfhsv);
	}

	TEST_METHOD(ConvertHslToHsv_WhenH240S100L50_Blue) {
		ColorFloatHSL cfhsl{240.f, 1.f, .5f};
		ColorFloatHSV expected{240.f, 1.f, 1.f};

		ColorFloatHSV cfhsv = cfhsl;

		Assert::AreEqual(expected, cfhsv);
	}

	TEST_METHOD(ConvertHslToHsv_WhenH264S53L40_H264S69V61) {
		ColorFloatHSL cfhsl{264.f, .53f, .4f};
		ColorFloatHSV expected{264.f, .69, .61f};

		ColorFloatHSV cfhsv = Color{cfhsl};

		Assert::AreEqual(expected.hue, cfhsv.hue, precision);
		Assert::AreEqual(expected.saturation, cfhsv.saturation, precision);
		Assert::AreEqual(expected.value, cfhsv.value, precision);
	}

	TEST_METHOD(ConvertHslToHsv_WhenH360S100L50_H360S100V5100) {
		ColorFloatHSL cfhsl{360.f, 1.f, .5f};
		ColorFloatHSV expected{360.f, 1.f, 1.f};

		ColorFloatHSV cfhsv = cfhsl;

		Assert::AreEqual(expected, cfhsv);
	}

	TEST_METHOD(ConvertHslToHsv_WhenHSLZero_Black) {
		ColorFloatHSL cfhsl;
		ColorFloatHSV expected;

		ColorFloatHSV cfhsv = cfhsl;

		Assert::AreEqual(expected, cfhsv);
	}

	TEST_METHOD(ConvertHsvToHsl_WhenH0S0V50_H0S0L50) {
		ColorFloatHSV cfhsv{0.f, 0.f, .5f};
		ColorFloatHSL expected{0.f, 0.f, .5f};

		ColorFloatHSL cfhsl = expected;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertHsvToHsl_WhenRed_H0S100L50) {
		ColorFloatHSV cfhsv{0.f, 1.f, 1.f};
		ColorFloatHSL expected{0.f, 1.f, .5f};

		ColorFloatHSL cfhsl = expected;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertHsvToHsl_WhenGreen_H120S100L50) {
		ColorFloatHSV cfhsv{120.f, 1.f, 1.f};
		ColorFloatHSL expected{120.f, 1.f, .5f};

		ColorFloatHSL cfhsl = expected;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertHsvToHsl_WhenBlue_H240S100L50) {
		ColorFloatHSV cfhsv{240.f, 1.f, 1.f};
		ColorFloatHSL expected{240.f, 1.f, .5f};

		ColorFloatHSL cfhsl = expected;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertHsvToHsl_WhenH264S69V61_H264S53L40) {
		ColorFloatHSV cfhsv{264.f, .69, .61f};
		ColorFloatHSL expected{264.f, .53f, .4f};

		ColorFloatHSL cfhsl = Color{expected};

		Assert::AreEqual(expected.hue, cfhsl.hue, precision);
		Assert::AreEqual(expected.saturation, cfhsl.saturation, precision);
		Assert::AreEqual(expected.lightness, cfhsl.lightness, precision);
	}

	TEST_METHOD(ConvertHsvToHsl_WhenH360S100V5100_H360S100L50) {
		ColorFloatHSV cfhsv{360.f, 1.f, 1.f};
		ColorFloatHSL expected{360.f, 1.f, .5f};

		ColorFloatHSL cfhsl = expected;

		Assert::AreEqual(expected, cfhsl);
	}

	TEST_METHOD(ConvertHsvToHsl_WhenHSLZero_Black) {
		ColorFloatHSV cfhsv;
		ColorFloatHSL expected;

		ColorFloatHSL cfhsl = expected;

		Assert::AreEqual(expected, cfhsl);
	}
};
