#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

import ScopeGuard;

using namespace shoujin;

TEST_CLASS(ScopeGuardTest) {
public:
	TEST_METHOD(GivenScope_WhenFuncIsNull_DoesNothing) {
		ScopeGuard<void> sg(nullptr);
	}

	TEST_METHOD(GivenScope_WhenGoingOutOfScope_CallFunction) {
		int callCount{};
		auto func = [](int* callCount) { ++(*callCount); };
		{
			ScopeGuard<int> sg(func, &callCount);
		}

		Assert::AreEqual(1, callCount);
	}
};
