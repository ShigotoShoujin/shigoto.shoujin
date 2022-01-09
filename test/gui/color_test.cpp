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
public:
	TEST_METHOD(ConstructColorByteRGB_CtorDefault_ColorZero) {
		const auto red = 0;
		const auto green = 0;
		const auto blue = 0;

		ColorByteRGB cbrgb;

		Assert::AreEqual<int>(red, cbrgb.red);
		Assert::AreEqual<int>(green, cbrgb.green);
		Assert::AreEqual<int>(blue, cbrgb.blue);
	}

	TEST_METHOD(ConstructColorByteRGB_CtorRGB_ColorIsRGB) {
		const auto red = 43;
		const auto green = 34;
		const auto blue = 122;

		ColorByteRGB cbrgb{red, green, blue};

		Assert::AreEqual<int>(red, cbrgb.red);
		Assert::AreEqual<int>(green, cbrgb.green);
		Assert::AreEqual<int>(blue, cbrgb.blue);
	}

	TEST_METHOD(ConstructColorByteRGB_CtorColorFloatRGB_ColorIsRGB) {
		const auto red = .22f;
		const auto green = .11f;
		const auto blue = .249f;

		ColorByteRGB cbrgb{ColorFloatRGB{red, green, blue}};

		Assert::AreEqual<int>(static_cast<int>(red * 255), cbrgb.red);
		Assert::AreEqual<int>(static_cast<int>(green * 255), cbrgb.green);
		Assert::AreEqual<int>(static_cast<int>(blue * 255), cbrgb.blue);
	}

	TEST_METHOD(ConstructColorFloatRGB_CtorDefault_ColorZero) {
		const auto red = .0f;
		const auto green = .0f;
		const auto blue = .0f;

		ColorFloatRGB cfrgb;

		Assert::AreEqual<float>(red, cfrgb.red);
		Assert::AreEqual<float>(green, cfrgb.green);
		Assert::AreEqual<float>(blue, cfrgb.blue);
	}

	TEST_METHOD(ConstructColorFloatRGB_CtorRGB_ColorIsRGB) {
		const auto red = .43f;
		const auto green = .34f;
		const auto blue = .122f;

		ColorFloatRGB cfrgb{red, green, blue};

		Assert::AreEqual<float>(red, cfrgb.red);
		Assert::AreEqual<float>(green, cfrgb.green);
		Assert::AreEqual<float>(blue, cfrgb.blue);
	}

	TEST_METHOD(ConstructColorFloatRGB_CtorColorByteRGB_ColorIsRGB) {
		const auto red = 22;
		const auto green = 11;
		const auto blue = 249;

		ColorFloatRGB cfrgb{ColorByteRGB{red, green, blue}};

		Assert::AreEqual(red / 255.f, cfrgb.red);
		Assert::AreEqual(green / 255.f, cfrgb.green);
		Assert::AreEqual(blue / 255.f, cfrgb.blue);
	}

	TEST_METHOD(ConstructColorByteHSL_CtorDefault_ColorZero) {
		const auto hue = 0;
		const auto saturation = 0;
		const auto lightness = 0;

		ColorByteHSL cfhsl;

		Assert::AreEqual<int>(hue, cfhsl.hue);
		Assert::AreEqual<int>(saturation, cfhsl.saturation);
		Assert::AreEqual<int>(lightness, cfhsl.lightness);
	}

	TEST_METHOD(ConstructColorByteHSL_CtorHSL_ColorIsHSL) {
		const auto hue = 200;
		const auto saturation = 70;
		const auto lightness = 80;

		ColorByteHSL cfhsl{hue, saturation, lightness};

		Assert::AreEqual<int>(hue, cfhsl.hue);
		Assert::AreEqual<int>(saturation, cfhsl.saturation);
		Assert::AreEqual<int>(lightness, cfhsl.lightness);
	}

	TEST_METHOD(ConstructColorByteHSL_CtorColorFloatHSL_ColorIsHSL) {
		const auto hue = 200.f;
		const auto saturation = .7f;
		const auto lightness = .8f;

		ColorByteHSL cfhsl{ColorFloatHSL{hue, saturation, lightness}};

		Assert::AreEqual<int>(static_cast<int>(hue), cfhsl.hue);
		Assert::AreEqual<int>(static_cast<int>(saturation * 100), cfhsl.saturation);
		Assert::AreEqual<int>(static_cast<int>(lightness * 100), cfhsl.lightness);
	}

	TEST_METHOD(ConstructColorFloatHSL_CtorDefault_ColorZero) {
		const auto hue = .0f;
		const auto saturation = .0f;
		const auto lightness = .0f;

		ColorFloatHSL cfhsl;

		Assert::AreEqual<float>(hue, cfhsl.hue);
		Assert::AreEqual<float>(saturation, cfhsl.saturation);
		Assert::AreEqual<float>(lightness, cfhsl.lightness);
	}

	TEST_METHOD(ConstructColorFloatHSL_CtorHSL_ColorIsHSL) {
		const auto hue = 200.f;
		const auto saturation = .70f;
		const auto lightness = .80f;

		ColorFloatHSL cfhsl{hue, saturation, lightness};

		Assert::AreEqual<float>(hue, cfhsl.hue);
		Assert::AreEqual<float>(saturation, cfhsl.saturation);
		Assert::AreEqual<float>(lightness, cfhsl.lightness);
	}

	TEST_METHOD(ConstructColorFloatHSL_CtorColorByteHSL_ColorIsHSL) {
		const auto hue = 200;
		const auto saturation = 70;
		const auto lightness = 80;

		ColorFloatHSL cfhsl{ColorByteHSL{hue, saturation, lightness}};

		Assert::AreEqual(hue / 1.f, cfhsl.hue);
		Assert::AreEqual(saturation / 100.f, cfhsl.saturation);
		Assert::AreEqual(lightness / 100.f, cfhsl.lightness);
	}

	TEST_METHOD(ConstructColorByteHSV_CtorDefault_ColorZero) {
		const auto hue = 0;
		const auto saturation = 0;
		const auto value = 0;

		ColorByteHSV cfhsv;

		Assert::AreEqual<int>(hue, cfhsv.hue);
		Assert::AreEqual<int>(saturation, cfhsv.saturation);
		Assert::AreEqual<int>(value, cfhsv.value);
	}

	TEST_METHOD(ConstructColorByteHSV_CtorHSV_ColorIsHSV) {
		const auto hue = 200;
		const auto saturation = 70;
		const auto value = 80;

		ColorByteHSV cfhsv{hue, saturation, value};

		Assert::AreEqual<int>(hue, cfhsv.hue);
		Assert::AreEqual<int>(saturation, cfhsv.saturation);
		Assert::AreEqual<int>(value, cfhsv.value);
	}

	TEST_METHOD(ConstructColorByteHSV_CtorColorFloatHSV_ColorIsHSV) {
		const auto hue = 200.f;
		const auto saturation = .7f;
		const auto value = .8f;

		ColorByteHSV cfhsv{ColorFloatHSV{hue, saturation, value}};

		Assert::AreEqual<int>(static_cast<int>(hue), cfhsv.hue);
		Assert::AreEqual<int>(static_cast<int>(saturation * 100), cfhsv.saturation);
		Assert::AreEqual<int>(static_cast<int>(value * 100), cfhsv.value);
	}

	TEST_METHOD(ConstructColorFloatHSV_CtorDefault_ColorZero) {
		const auto hue = .0f;
		const auto saturation = .0f;
		const auto value = .0f;

		ColorFloatHSV cfhsv;

		Assert::AreEqual<float>(hue, cfhsv.hue);
		Assert::AreEqual<float>(saturation, cfhsv.saturation);
		Assert::AreEqual<float>(value, cfhsv.value);
	}

	TEST_METHOD(ConstructColorFloatHSV_CtorHSV_ColorIsHSV) {
		const auto hue = 200.f;
		const auto saturation = .70f;
		const auto value = .80f;

		ColorFloatHSV cfhsv{hue, saturation, value};

		Assert::AreEqual<float>(hue, cfhsv.hue);
		Assert::AreEqual<float>(saturation, cfhsv.saturation);
		Assert::AreEqual<float>(value, cfhsv.value);
	}

	TEST_METHOD(ConstructColorFloatHSV_CtorColorByteHSV_ColorIsHSV) {
		const auto hue = 200;
		const auto saturation = 70;
		const auto value = 80;

		ColorFloatHSV cfhsv{ColorByteHSV{hue, saturation, value}};

		Assert::AreEqual(hue / 1.f, cfhsv.hue);
		Assert::AreEqual(saturation / 100.f, cfhsv.saturation);
		Assert::AreEqual(value / 100.f, cfhsv.value);
	}

	TEST_METHOD(ConstructColor_CtorDefault_AllZero) {
		Color color;

		ColorFloatRGB actual = color;

		Assert::AreEqual<float>(0, actual.red);
		Assert::AreEqual<float>(0, actual.green);
		Assert::AreEqual<float>(0, actual.blue);
	}

	TEST_METHOD(ConstructColor_CtorCOLORREF_ColorAssigned) {
		const auto red = 245;
		const auto green = 39;
		const auto blue = 167;

		ColorFloatRGB actual = Color(RGB(245, 39, 167));

		Assert::AreEqual<float>(red / 255.f, actual.red);
		Assert::AreEqual<float>(green / 255.f, actual.green);
		Assert::AreEqual<float>(blue / 255.f, actual.blue);
	}

	TEST_METHOD(ConstructColor_CtorColorByteRGB_ColorAssigned) {
		const auto red = 245;
		const auto green = 39;
		const auto blue = 167;

		ColorFloatRGB actual = Color(ColorByteRGB{red, green, blue});

		Assert::AreEqual<float>(red / 255.f, actual.red);
		Assert::AreEqual<float>(green / 255.f, actual.green);
		Assert::AreEqual<float>(blue / 255.f, actual.blue);
	}

	TEST_METHOD(ConstructColor_CtorColorFloatRGB_ColorAssigned) {
		const auto red = 245 / 255.f;
		const auto green = 39 / 255.f;
		const auto blue = 167 / 255.f;

		ColorFloatRGB actual = Color(ColorFloatRGB{red, green, blue});

		Assert::AreEqual(red, actual.red);
		Assert::AreEqual(green, actual.green);
		Assert::AreEqual(blue, actual.blue);
	}

	TEST_METHOD(ConstructColor_CtorColorByteHSL_ColorAssigned) {
		const auto hue = 0;
		const auto saturation = 0;
		const auto lightness = 50;

		ColorFloatRGB actual = Color(ColorByteHSL{hue, saturation, lightness});

		Assert::AreEqual(.5f, actual.red);
		Assert::AreEqual(.5f, actual.green);
		Assert::AreEqual(.5f, actual.blue);
	}

	TEST_METHOD(ConstructColor_CtorColorFloatHSL_ColorAssigned) {
		const auto hue = .0f;
		const auto saturation = .0f;
		const auto lightness = .5f;

		ColorFloatRGB actual = Color(ColorFloatHSL{hue, saturation, lightness});

		Assert::AreEqual(.5f, actual.red);
		Assert::AreEqual(.5f, actual.green);
		Assert::AreEqual(.5f, actual.blue);
	}

	TEST_METHOD(ConstructColor_CtorColorByteHSV_ColorAssigned) {
		const auto hue = 0;
		const auto saturation = 0;
		const auto lightness = 50;

		ColorFloatRGB actual = Color(ColorByteHSV{hue, saturation, lightness});

		Assert::AreEqual(.5f, actual.red);
		Assert::AreEqual(.5f, actual.green);
		Assert::AreEqual(.5f, actual.blue);
	}

	TEST_METHOD(ConstructColor_CtorColorFloatHSV_ColorAssigned) {
		const auto hue = .0f;
		const auto saturation = .0f;
		const auto lightness = .5f;

		ColorFloatRGB actual = Color(ColorFloatHSV{hue, saturation, lightness});

		Assert::AreEqual(.5f, actual.red);
		Assert::AreEqual(.5f, actual.green);
		Assert::AreEqual(.5f, actual.blue);
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

	TEST_METHOD(OperatorConvertToColorByteRGB_ColorMatch) {
		const auto red = 245;
		const auto green = 39;
		const auto blue = 167;

		ColorByteRGB actual = Color(ColorByteRGB(red, green, blue));

		Assert::AreEqual<int>(red, actual.red);
		Assert::AreEqual<int>(green, actual.green);
		Assert::AreEqual<int>(blue, actual.blue);
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

	TEST_METHOD(OperatorConvertToColorByteHSL_ColorMatch) {
		const auto red = 128;
		const auto green = 128;
		const auto blue = 128;

		ColorByteHSL actual = Color(ColorByteRGB(red, green, blue));

		Assert::AreEqual<int>(0, actual.hue);
		Assert::AreEqual<int>(0, actual.saturation);
		Assert::AreEqual<int>(50, actual.lightness);
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

	TEST_METHOD(OperatorConvertToColorByteHSV_ColorMatch) {
		const auto red = 128;
		const auto green = 128;
		const auto blue = 128;

		ColorByteHSV actual = Color(ColorByteRGB(red, green, blue));

		Assert::AreEqual<int>(0, actual.hue);
		Assert::AreEqual<int>(0, actual.saturation);
		Assert::AreEqual<int>(50, actual.value);
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

	TEST_METHOD(ConvertRgbToHsl_WhenBlack_HSLZero) {
		Color color;
		ColorByteHSL expected;

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenRed_H0S100L50) {
		Color color = ColorByteRGB(255, 0, 0);
		ColorByteHSL expected{0, 100, 50};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenGreen_H120S100L50) {
		Color color = ColorByteRGB(0, 255, 0);
		ColorByteHSL expected{120, 100, 50};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenBlue_H240S100L50) {
		Color color = ColorByteRGB(0, 0, 255);
		ColorByteHSL expected{240, 100, 50};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenWhite_H0S0L100) {
		Color color = ColorByteRGB(255, 255, 255);
		ColorByteHSL expected{0, 0, 100};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenGray_H0S0L50) {
		Color color = ColorByteRGB(128, 128, 128);
		ColorByteHSL expected{0, 0, 50};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenR91G48B156_H264S53L40) {
		Color color = ColorByteRGB(91, 48, 156);
		ColorByteHSL expected{264, 53, 40};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertRgbToHsl_WhenR255G254B255_H300S100L100) {
		Color color = ColorByteRGB(255, 254, 255);
		ColorByteHSL expected{300, 100, 100};

		ColorByteHSL cbhsl = color;

		Assert::AreEqual(expected, cbhsl);
	}

	TEST_METHOD(ConvertHslToRgb_WhenHSLZero_Black) {
		ColorFloatHSL cfhsl;
		Color expected;

		Color color = cfhsl;

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

	TEST_METHOD(ConvertHslToRgb_WhenH0S0L50_R128G128B128) {
		ColorFloatHSL cfhsl{0.f, 0.f, .5f};
		ColorByteRGB expected{128, 128, 128};

		ColorByteRGB color = Color(cfhsl);

		Assert::AreEqual(expected, color);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenBlack_HSVZero) {
		Color color;
		ColorByteHSV expected;

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenRed_H0S100L50) {
		Color color = ColorByteRGB(255, 0, 0);
		ColorByteHSV expected{0, 100, 100};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenGreen_H120S100L50) {
		Color color = ColorByteRGB(0, 255, 0);
		ColorByteHSV expected{120, 100, 100};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenBlue_H240S100L50) {
		Color color = ColorByteRGB(0, 0, 255);
		ColorByteHSV expected{240, 100, 100};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenWhite_H0S0L100) {
		Color color = ColorByteRGB(255, 255, 255);
		ColorByteHSV expected{0, 0, 100};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenGray_H0S0L50) {
		Color color = ColorByteRGB(128, 128, 128);
		ColorByteHSV expected{0, 0, 50};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenR91G48B156_H264S53L40) {
		Color color = ColorByteRGB(91, 48, 156);
		ColorByteHSV expected{264, 69, 61};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertRgbToHsv_WhenR255G254B255_H300S100L100) {
		Color color = ColorByteRGB(255, 254, 255);
		ColorByteHSV expected{300, 0, 100};

		ColorByteHSV cbhsv = color;

		Assert::AreEqual(expected, cbhsv);
	}

	TEST_METHOD(ConvertHsvToRgb_WhenHSVZero_Black) {
		ColorFloatHSV cfhsv;
		ColorByteRGB expected;

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

	TEST_METHOD(ConvertHsvToRgb_WhenH0S0L50_R128G128B128) {
		ColorFloatHSV cfhsv{0.f, 0.f, .502f};
		ColorByteRGB expected{128, 128, 128};

		ColorByteRGB color = Color(cfhsv);

		Assert::AreEqual(expected, color);
	}
};
