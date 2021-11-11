#include <shoujin/assert.hpp>
#include <shoujin/gui.hpp>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin::gui;

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

	static bool OnError(const shoujin::assert::ErrorInfo& ei, void* userdata)
	{
		int* count = reinterpret_cast<int*>(userdata);
		++*count;
		return true;
	}

	TEST_METHOD(BitmapTest_CreateWithSizeZero_Assert) {
		//Arrange
		int count{};
		shoujin::assert::OnErrorEvent = {OnError, &count};

		//Act
		Bitmap bitmap{{}};

		//Assert
		Assert::AreEqual(2, count);

		//Cleanup
		shoujin::assert::OnErrorEvent = nullptr;
	}

	TEST_METHOD(BitmapTest_CopyConstructor) {
		//Arrange
		Size size{32, 32};
		Bitmap source(size);

		//Act
		Bitmap copy(source);

		//Assert
		Assert::IsNotNull(source.hdc());
		Assert::IsTrue(size == source.size());

		Assert::IsNotNull(copy.hdc());
		Assert::IsFalse(source.hdc() == copy.hdc());
		Assert::IsTrue(size == copy.size());
	}

	TEST_METHOD(BitmapTest_CopyAssignment) {
		//Arrange
		Bitmap first({16, 16});
		Bitmap second({32, 32});

		//Act
		second = first;

		//Assert
		Assert::IsNotNull(first.hdc());
		Assert::IsTrue(Size{16, 16} == first.size());

		Assert::IsFalse(first.hdc() == second.hdc());

		Assert::IsNotNull(second.hdc());
		Assert::IsTrue(Size{16, 16} == second.size());
	}

	TEST_METHOD(BitmapTest_MoveConstructor) {
		//Arrange
		Size size{32, 32};
		Bitmap source(size);

		//Act
		Bitmap moved(std::move(source));

		//Assert
#pragma warning(suppress : 26800)
		Assert::IsNull(source.hdc());
		Assert::IsTrue(Size{0, 0} == source.size());

		Assert::IsNotNull(moved.hdc());
		Assert::IsTrue(size == moved.size());
	}

	TEST_METHOD(BitmapTest_MoveAssignment) {
		//Arrange
		Bitmap first({16, 16});
		Bitmap second({32, 32});

		//Act
		second = std::move(first);

		//Assert
#pragma warning(suppress : 26800)
		Assert::IsNull(first.hdc());
		Assert::IsTrue(Size{0, 0} == first.size());

		Assert::IsNotNull(second.hdc());
		Assert::IsTrue(Size{16, 16} == second.size());
	}
};
