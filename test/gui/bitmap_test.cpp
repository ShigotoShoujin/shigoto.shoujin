#include <shoujin/gui.hpp>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::gui;

inline bool operator==(const SIZE& first, const SIZE& second)
{
	return first.cx == second.cx && first.cy == second.cy;
}

TEST_CLASS(BitmapTest) {
public:
	TEST_METHOD(BitmapTest_IsCopyConstructible) {
		Assert::IsTrue(std::is_copy_constructible_v<Bitmap>);
	}

	TEST_METHOD(BitmapTest_IsCopyAssignable) {
		Assert::IsTrue(std::is_copy_assignable_v<Bitmap>);
	}

	TEST_METHOD(BitmapTest_IsMoveConstructible) {
		Assert::IsTrue(std::is_move_constructible_v<Bitmap>);
	}

	TEST_METHOD(BitmapTest_IsMoveAssignable) {
		Assert::IsTrue(std::is_move_assignable_v<Bitmap>);
	}

	TEST_METHOD(BitmapTest_CopyConstructor) {
		//Arrange
		SIZE size{32, 32};
		Bitmap source(size);

		//Act
		Bitmap copy(source);

		//Assert
		Assert::IsNotNull(source.HandleDC());
		Assert::IsTrue(size == source.BitmapSize());

		Assert::IsNotNull(copy.HandleDC());
		Assert::IsFalse(source.HandleDC() == copy.HandleDC());
		Assert::IsTrue(size == copy.BitmapSize());
	}

	TEST_METHOD(BitmapTest_CopyAssignment) {
		//Arrange
		Bitmap first({16, 16});
		Bitmap second({32, 32});

		//Act
		second = first;

		//Assert
		Assert::IsNotNull(first.HandleDC());
		Assert::IsTrue(SIZE{16, 16} == first.BitmapSize());

		Assert::IsFalse(first.HandleDC() == second.HandleDC());

		Assert::IsNotNull(second.HandleDC());
		Assert::IsTrue(SIZE{16, 16} == second.BitmapSize());
	}

	TEST_METHOD(BitmapTest_MoveConstructor) {
		//Arrange
		SIZE size{32, 32};
		Bitmap source(size);

		//Act
		Bitmap moved(std::move(source));

		//Assert
		Assert::IsNull(source.HandleDC());
		Assert::IsTrue(SIZE{0, 0} == source.BitmapSize());

		Assert::IsNotNull(moved.HandleDC());
		Assert::IsTrue(size == moved.BitmapSize());
	}

	TEST_METHOD(BitmapTest_MoveAssignment) {
		//Arrange
		Bitmap first({16, 16});
		Bitmap second({32, 32});

		//Act
		second = std::move(first);

		//Assert
		Assert::IsNull(first.HandleDC());
		Assert::IsTrue(SIZE{0, 0} == first.BitmapSize());

		Assert::IsNotNull(second.HandleDC());
		Assert::IsTrue(SIZE{16, 16} == second.BitmapSize());
	}
};
