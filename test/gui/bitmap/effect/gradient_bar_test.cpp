#include "../../../../src/gui/bitmap/effect/gradient_bar.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace shoujin::gui::bitmap::effect;

namespace Microsoft::VisualStudio::CppUnitTestFramework {

bool operator==(GradientBar::Color const& lhs, GradientBar::Color const& rhs)
{
	return lhs.R == rhs.R && lhs.G == rhs.G && lhs.B == rhs.B;
}

template<> inline std::wstring ToString<GradientBar::Color>(GradientBar::Color const& t)
{
	std::wstringstream wss;
	wss << L"Color RGB: (" << t.R << L',' << t.G << L',' << t.B << L')';
	return wss.str();
}

}

TEST_CLASS(GradientBarTest) {
public:
	TEST_METHOD(GradientBar_SizeSmallerThanSeven_AllBlack) {
		//Arrange
		GradientBar gb(6);
		const int kSampleSize = 7;
		decltype(gb.NextPixel()) color[kSampleSize];

		//Act
		for(int i = 0; i < kSampleSize; ++i)
			color[i] = gb.NextPixel();

		//Assert
		for(int i = 0; i < kSampleSize; ++i)
			Assert::AreEqual({}, color[i]);
	}

	TEST_METHOD(GradientBar_SizeSeven_FillGradient) {
		//Arrange
		GradientBar gb(7);
		const int kSampleSize = 8;
		decltype(gb.NextPixel()) color[kSampleSize];

		//Act
		for(int i = 0; i < kSampleSize; ++i)
			color[i] = gb.NextPixel();

		//Assert
		Assert::AreEqual({255, 0, 0}, color[0]);
		Assert::AreEqual({255, 0, 255}, color[1]);
		Assert::AreEqual({0, 0, 255}, color[2]);
		Assert::AreEqual({0, 255, 255}, color[3]);
		Assert::AreEqual({0, 255, 0}, color[4]);
		Assert::AreEqual({255, 255, 0}, color[5]);
		Assert::AreEqual({255, 0, 0}, color[6]);
		Assert::AreEqual({}, color[7]);
	}

	TEST_METHOD(GradientBar_IdealSize_FillGradient) {
		//Arrange
		const int kPhase = 6;
		const int kMaxColor = 0xff;
		const int kSize = kMaxColor * kPhase + 1; //+1 because it start at zero
		GradientBar gb(kSize);
		decltype(gb.NextPixel()) color[kSize];

		//Act
		for(int i = 0; i < kSize; ++i) {
			auto p = gb.NextPixel();
			color[i] = p;
		}

		//Assert
		int offset = 0;
		GradientBar::Color c{255, 0, 0};

		//Blue++
		for(int i = 0; i < 0xff; ++i) {
			Assert::AreEqual(c, color[i + offset]);
			++c.B;
		}

		//Red--
		offset += 0xff;
		for(int i = 0; i < 0xff; ++i) {
			Assert::AreEqual(c, color[i + offset]);
			--c.R;
		}

		//Green++
		offset += 0xff;
		for(int i = 0; i < 0xff; ++i) {
			Assert::AreEqual(c, color[i + offset]);
			++c.G;
		}

		//Blue--
		offset += 0xff;
		for(int i = 0; i < 0xff; ++i) {
			Assert::AreEqual(c, color[i + offset]);
			--c.B;
		}

		//Red++
		offset += 0xff;
		for(int i = 0; i < 0xff; ++i) {
			Assert::AreEqual(c, color[i + offset]);
			++c.R;
		}
	}
};
