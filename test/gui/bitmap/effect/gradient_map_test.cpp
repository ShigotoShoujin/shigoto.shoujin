#include "../../../../src/gui/bitmap/effect/gradient_map.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::gui::bitmap::effect;

namespace Microsoft::VisualStudio::CppUnitTestFramework {

bool operator==(GradientMap::Color const& lhs, GradientMap::Color const& rhs)
{
	return lhs.R == rhs.R && lhs.G == rhs.G && lhs.B == rhs.B;
}

template<> inline std::wstring ToString<GradientMap::Color>(GradientMap::Color const& t)
{
	std::wstringstream wss;
	wss << L"Color RGB: (" << t.R << L',' << t.G << L',' << t.B << L')';
	return wss.str();
}

}

TEST_CLASS(GradientMapTest) {
public:
	TEST_METHOD(GradientMapAllBlack_AllBlack) {
		GradientMap::Color black{0, 0, 0};
		GradientMap gradient{{black, black, black, black}, 10, 10};

		for(auto y = 0; y < 10; ++y) {
			gradient.NextRow();
			for(auto x = 0; x < 10; ++x)
				Assert::AreEqual(black, gradient.NextPixel());
		}
	}

	TEST_METHOD(GradientMapRedRedBlackBlack_TopToBottomAreLinear) {
		GradientMap::Color red{0xff, 0, 0};
		GradientMap::Color black{0, 0, 0};
		GradientMap gradient{{red, red, black, black}, 4, 4};

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

	TEST_METHOD(GradientMapAllFourSet_EachCorderDifferent) {
		GradientMap::Color top_left{100, 0, 0};
		GradientMap::Color top_right{160, 0, 0};
		GradientMap::Color bottom_left{52, 0, 0};
		GradientMap::Color bottom_right{242, 0, 0};
		GradientMap gradient{{top_left, top_right, bottom_left, bottom_right}, 4, 4};

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
