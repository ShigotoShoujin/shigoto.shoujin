#include <ui/ui.hpp>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
		Assert::IsNotNull(source.GetDC());
		Assert::IsTrue(size == source.GetSize());

		Assert::IsNotNull(copy.GetDC());
		Assert::IsFalse(source.GetDC() == copy.GetDC());
		Assert::IsTrue(size == copy.GetSize());
	}

	TEST_METHOD(BitmapTest_CopyAssignment) {
		//Arrange
		Bitmap first({16, 16});
		Bitmap second({32, 32});

		//Act
		second = first;

		//Assert
		Assert::IsNotNull(first.GetDC());
		Assert::IsTrue(SIZE{16, 16} == first.GetSize());

		Assert::IsFalse(first.GetDC() == second.GetDC());

		Assert::IsNotNull(second.GetDC());
		Assert::IsTrue(SIZE{16, 16} == second.GetSize());
	}

	TEST_METHOD(BitmapTest_MoveConstructor) {
		//Arrange
		SIZE size{32, 32};
		Bitmap source(size);

		//Act
		Bitmap moved(std::move(source));

		//Assert
		Assert::IsNull(source.GetDC());
		Assert::IsTrue(SIZE{0, 0} == source.GetSize());

		Assert::IsNotNull(moved.GetDC());
		Assert::IsTrue(size == moved.GetSize());
	}

	TEST_METHOD(BitmapTest_MoveAssignment) {
		//Arrange
		Bitmap first({16, 16});
		Bitmap second({32, 32});

		//Act
		second = std::move(first);

		//Assert
		Assert::IsNull(first.GetDC());
		Assert::IsTrue(SIZE{0, 0} == first.GetSize());

		Assert::IsNotNull(second.GetDC());
		Assert::IsTrue(SIZE{16, 16} == second.GetSize());
	}
};
