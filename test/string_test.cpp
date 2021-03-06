import Shoujin.String;

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace shoujin;
using namespace shoujin::string;

namespace shoujin::test::string {

TEST_CLASS(StringTest) {
	char const* charValue = "hello";
	wchar_t const* wcharValue = L"hello";
	std::string strValue = "hello";
	std::wstring wstrValue = L"hello";

public:
#ifdef UNICODE
	TEST_METHOD(string_ConstructionFromStringViewWide_IsWideString) {
		String str = StringView{wcharValue};
		Assert::AreEqual(wcharValue, str.c_str());
	}

	TEST_METHOD(string_ConstructionAssignmentShortString_IsWideString) {
		String str = charValue;
		Assert::AreEqual(wcharValue, str.c_str());
	}

	TEST_METHOD(string_ConstructionAssignmentWideString_IsWideString) {
		String str = wcharValue;
		Assert::AreEqual(wcharValue, str.c_str());
	}

	TEST_METHOD(string_AssignmentOperatorShortString_IsWideString) {
		String str;
		str = charValue;
		Assert::AreEqual(wcharValue, str.c_str());
	}

	TEST_METHOD(string_AssignmentOperatorWideString_IsWideString) {
		String str;
		str = wcharValue;
		Assert::AreEqual(wcharValue, str.c_str());
	}

	TEST_METHOD(string_FromShortString_IsWideString) {
		String str{charValue};

		Assert::AreEqual(wcharValue, str.c_str());
	}

	TEST_METHOD(string_FromWideString_IsWideString) {
		String str{wcharValue};

		Assert::AreEqual(wcharValue, str.c_str());
	}

	TEST_METHOD(toTString_CHAR_ResultIsWide) {
		auto&& actual = ToTString(charValue);
		Assert::AreEqual(wcharValue, actual.c_str());
	}

	TEST_METHOD(toTString_WCHAR_ResultIsWide) {
		auto&& actual = ToTString(charValue);
		Assert::AreEqual(wcharValue, actual.c_str());
	}

	TEST_METHOD(toTString_String_ResultIsWide) {
		auto&& actual = ToTString(strValue);
		Assert::AreEqual(wstrValue.c_str(), actual.c_str());
	}

	TEST_METHOD(toTString_WString_ResultIsWide) {
		auto&& actual = ToTString(wstrValue);
		Assert::AreEqual(wstrValue, actual);
	}
#else
	TEST_METHOD(string_ConstructionFromStringViewShort_IsShortString) {
		String str = StringView{charValue};
		Assert::AreEqual(charValue, str.c_str());
	}

	TEST_METHOD(string_ConstructionAssignmentShortString_IsShortString) {
		String str = charValue;
		Assert::AreEqual(charValue, str.c_str());
	}

	TEST_METHOD(string_ConstructionAssignmentWideString_IsShortString) {
		String str = wcharValue;
		Assert::AreEqual(charValue, str.c_str());
	}

	TEST_METHOD(string_AssignmentOperatorShortString_IsShortString) {
		String str;
		str = charValue;
		Assert::AreEqual(charValue, str.c_str());
	}

	TEST_METHOD(string_AssignmentOperatorWideString_IsShortString) {
		String str;
		str = wcharValue;
		Assert::AreEqual(charValue, str.c_str());
	}

	TEST_METHOD(string_FromShortString_IsShortString) {
		String str{charValue};

		Assert::AreEqual(charValue, str.c_str());
	}

	TEST_METHOD(string_FromWideString_IsShortString) {
		String str{wcharValue};

		Assert::AreEqual(charValue, str.c_str());
	}

	TEST_METHOD(toTString_CHAR_ResultIsShort) {
		auto&& actual = ToTString(charValue);
		Assert::AreEqual(charValue, actual.c_str());
	}

	TEST_METHOD(toTString_WCHAR_ResultIsShort) {
		auto&& actual = ToTString(charValue);
		Assert::AreEqual(charValue, actual.c_str());
	}

	TEST_METHOD(toTString_String_ResultIsShort) {
		auto&& actual = ToTString(strValue);
		Assert::AreEqual(strValue, actual);
	}

	TEST_METHOD(toTString_WString_ResultIsShort) {
		auto&& actual = ToTString(wstrValue);
		Assert::AreEqual(strValue.c_str(), actual.c_str());
	}
#endif

	TEST_METHOD(toMbString_CHAR_ResultIsShort) {
		auto&& actual = ToMbString(charValue);
		Assert::AreEqual(charValue, actual.c_str());
	}

	TEST_METHOD(toMbString_WCHAR_ResultIsShort) {
		auto&& actual = ToMbString(wcharValue);
		Assert::AreEqual(charValue, actual.c_str());
	}

	TEST_METHOD(toMbString_String_ResultIsShort) {
		auto&& actual = ToMbString(strValue);
		Assert::AreEqual(strValue, actual);
	}

	TEST_METHOD(toMbString_WString_ResultIsShort) {
		auto&& actual = ToMbString(wstrValue);
		Assert::AreEqual(strValue.c_str(), actual.c_str());
	}

	TEST_METHOD(toWideString_CHAR_ResultIsWide) {
		auto&& actual = ToWideString(charValue);
		Assert::AreEqual(wcharValue, actual.c_str());
	}

	TEST_METHOD(toWideString_WCHAR_ResultIsWide) {
		auto&& actual = ToWideString(wcharValue);
		Assert::AreEqual(wcharValue, actual.c_str());
	}

	TEST_METHOD(toWideString_String_ResultIsWide) {
		auto&& actual = ToWideString(strValue);
		Assert::AreEqual(wstrValue.c_str(), actual.c_str());
	}

	TEST_METHOD(toWideString_WString_ResultIsWide) {
		auto&& actual = ToWideString(wstrValue);
		Assert::AreEqual(wstrValue, actual);
	}
};

}
