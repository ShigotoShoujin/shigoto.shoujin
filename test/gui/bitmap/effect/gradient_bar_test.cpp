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
		const int kSize = 6;
		const int kSampleSize = kSize + 1;
		GradientBar gb(kSize);
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
		const int kSize = 7;
		const int kSampleSize = kSize + 1;
		GradientBar gb(kSize);
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

	TEST_METHOD(GradientBar_Size32_FillGradient) {
		//Arrange
		const int kSize = 32;
		GradientBar gb(kSize);
		decltype(gb.NextPixel()) color[kSize];

		//Act
		for(int i = 0; i < kSize; ++i)
			color[i] = gb.NextPixel();

		//Assert
		Assert::AreEqual({255, 0, 0}, color[0]);
		Assert::AreEqual({255, 0, 49}, color[1]);
		Assert::AreEqual({255, 0, 247}, color[5]);
		Assert::AreEqual({206, 0, 255}, color[6]);
		Assert::AreEqual({8, 0, 255}, color[10]);
		Assert::AreEqual({0, 49, 255}, color[11]);
		Assert::AreEqual({0, 247, 255}, color[15]);
		Assert::AreEqual({0, 255, 206}, color[16]);
		Assert::AreEqual({0, 255, 8}, color[20]);
		Assert::AreEqual({49, 255, 0}, color[21]);
		Assert::AreEqual({247, 255, 0}, color[25]);
		Assert::AreEqual({255, 206, 0}, color[26]);
		Assert::AreEqual({255, 8, 0}, color[30]);
		Assert::AreEqual({255, 0, 0}, color[31]);
	}

	TEST_METHOD(GradientBar_Size380_FillGradient) {
		//Arrange
		const int kSize = 380;
		GradientBar gb(kSize);
		decltype(gb.NextPixel()) color[kSize];

		//Act
		for(int i = 0; i < kSize; ++i)
			color[i] = gb.NextPixel();

		//Assert
		Assert::AreEqual({255, 0, 0}, color[0]);
		Assert::AreEqual({255, 0, 4}, color[1]);
		Assert::AreEqual({255, 0, 254}, color[63]);
		Assert::AreEqual({251, 0, 255}, color[64]);

		Assert::AreEqual({5, 0, 255}, color[125]);
		Assert::AreEqual({1, 0, 255}, color[126]);
		Assert::AreEqual({0, 4, 255}, color[127]);

		Assert::AreEqual({0, 254, 255}, color[189]);
		Assert::AreEqual({0, 255, 251}, color[190]);

		Assert::AreEqual({0, 255, 1}, color[252]);
		Assert::AreEqual({4, 255, 0}, color[253]);

		Assert::AreEqual({254, 255, 0}, color[315]);
		Assert::AreEqual({255, 251, 0}, color[316]);
		Assert::AreEqual({255, 1, 0}, color[378]);
		Assert::AreEqual({255, 0, 0}, color[379]);
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
