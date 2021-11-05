#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <any>
#include <shoujin/assert.hpp>

using namespace shoujin;
using namespace shoujin::assert;

TEST_CLASS(AssertTest) {
	struct TestData {
		struct EventData {
			int callcount{};
			bool cancel{};
		} onerror, onerroroutput, onexitprocess;
		ErrorInfo ei{};
	};

	static void UpdateEventData(TestData::EventData & event_data, bool& cancel)
	{
		if(event_data.cancel)
			cancel = true;

		++event_data.callcount;
	}

	static void OnError(const ErrorInfo& ei, bool& cancel, void* userdata)
	{
		auto testdata = reinterpret_cast<TestData*>(userdata);
		testdata->ei = ei;
		UpdateEventData(testdata->onerror, cancel);
	}

	static void OnErrorOutput(tstring error_message, bool& cancel, void* userdata)
	{
		UpdateEventData(reinterpret_cast<TestData*>(userdata)->onerroroutput, cancel);
	}

	static void OnExitProcess(bool& cancel, void* userdata)
	{
		UpdateEventData(reinterpret_cast<TestData*>(userdata)->onexitprocess, cancel);
	}

public:
	TEST_METHOD_CLEANUP(TestCleanup)
	{
		OnErrorEvent = nullptr;
		OnErrorOutputEvent = nullptr;
		OnExitProcessEvent = nullptr;
	}

	TEST_METHOD(Assert_OnErrorCancelled_DoesNotRaiseOutputErrorEvent) {
		TestData testdata{.onerror{.cancel = true}};
		OnErrorEvent = {OnError, &testdata};
		OnErrorOutputEvent = {OnErrorOutput, &testdata};
		OnExitProcessEvent = {OnExitProcess, &testdata};

		SHOUJIN_ASSERT(("This assertion will fail", 0));

		Assert::AreEqual(1, testdata.onerror.callcount);
		Assert::AreEqual(0, testdata.onerroroutput.callcount);
		Assert::AreEqual(0, testdata.onexitprocess.callcount);
	}

	TEST_METHOD(Assert_OnErrorOutputCancelled_DoesNotRaiseExitProcessEvent) {
		TestData testdata{.onerroroutput{.cancel = true}};
		OnErrorEvent = {OnError, &testdata};
		OnErrorOutputEvent = {OnErrorOutput, &testdata};
		OnExitProcessEvent = {OnExitProcess, &testdata};

		SHOUJIN_ASSERT(("This assertion will fail", 0));

		Assert::AreEqual(1, testdata.onerror.callcount);
		Assert::AreEqual(1, testdata.onerroroutput.callcount);
		Assert::AreEqual(0, testdata.onexitprocess.callcount);
	}

	TEST_METHOD(Assert_OnExitProcessCancelled_DoesNotActuallyExitProcess) {
		TestData testdata{.onexitprocess{.cancel = true}};
		OnErrorEvent = {OnError, &testdata};
		OnErrorOutputEvent = {OnErrorOutput, &testdata};
		OnExitProcessEvent = {OnExitProcess, &testdata};

		SHOUJIN_ASSERT(("This assertion will fail", 0));

		Assert::AreEqual(1, testdata.onerror.callcount);
		Assert::AreEqual(1, testdata.onerroroutput.callcount);
		Assert::AreEqual(1, testdata.onexitprocess.callcount);
	}

	TEST_METHOD(Assert_OnErrorRaised_ParametersOk) {
		TestData testdata{.onerror{.cancel = true}};
		ErrorInfo& ei = testdata.ei;
		OnErrorEvent = {OnError, &testdata};
		const int kFailCode = 8;

		auto line = __LINE__ + 1;
		SHOUJIN_ASSERT_EXPLICIT(("This assertion will fail", kFailCode), [kFailCode](int ret_code) { return ret_code != kFailCode; });

		Assert::IsTrue(ei.file.ends_with(TEXT("assert_test.cpp")));
		Assert::IsTrue(ei.function.starts_with(TEXT("AssertTest::Assert_OnErrorRaised_ParametersOk::")));
		Assert::IsTrue(line == ei.line);
		Assert::IsTrue(ei.expression == TEXT("(\"This assertion will fail\", kFailCode)"));
		Assert::IsTrue(any_cast<int>(ei.result) == kFailCode);
	}
};
