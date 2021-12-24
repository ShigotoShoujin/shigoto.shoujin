#include "../../../../src/gui/bitmap/effect/gradient.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::gui::bitmap::effect;

bool operator==(Gradient::Color const& lhs, Gradient::Color const& rhs)
{
	return lhs.R == rhs.R && lhs.G == rhs.G && lhs.B == rhs.B;
}

TEST_CLASS(GradientTest) {
public:
	TEST_METHOD(GradientAllBlack_AllBlack) {
		Gradient::Color black{0, 0, 0};
		Gradient gradient{{black, black, black, black}, 10, 10};

		for(auto y = 0; y < 10; ++y) {
			gradient.NextRow();
			for(auto x = 0; x < 10; ++x)
				Assert::IsTrue(black == gradient.NextPixel());
		}
	}

	TEST_METHOD(GradientRedRedBlackBlack_SpotCheckOk) {
		Gradient::Color red{0xff, 0, 0};
		Gradient::Color black{0, 0, 0};
		Gradient gradient{{red, red, black, black}, 4, 4};

		// clang-format off
		int expected[] {
			255, 255, 255, 255,
			170, 170, 170, 170,
			 85,  85,  85,  85,
			  0,   0,   0,   0,
		};
		// clang-format on

		auto i = 0;
		for(auto y = 0; y < 4; ++y) {
			gradient.NextRow();
			for(auto x = 0; x < 4; ++x)
				Assert::AreEqual<int>(expected[i++], gradient.NextPixel().R);
		}
	}

	TEST_METHOD(GradientAllFourSet_SpotCheckOk) {
		Gradient::Color top_left{100, 0, 0};
		Gradient::Color top_right{160, 0, 0};
		Gradient::Color bottom_left{52, 0, 0};
		Gradient::Color bottom_right{242, 0, 0};
		Gradient gradient{{top_left, top_right, bottom_left, bottom_right}, 4, 4};

		// clang-format off
		int expected[] {
			100, 120, 140, 160,
			 84, 118, 152, 187,
			 68, 116, 165, 214,
			 52, 115, 178, 241,
		};
		// clang-format on

		auto i = 0;
		for(auto y = 0; y < 4; ++y) {
			gradient.NextRow();
			for(auto x = 0; x < 4; ++x)
				Assert::AreEqual<int>(expected[i++], gradient.NextPixel().R);
		}
	}
};
