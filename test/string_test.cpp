#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <shoujin/tstring.hpp>
#include <utility>

using namespace shoujin;

class String : public tstring {
public:
	String() = default;

	String(const std::string& tstr) :
		tstring(ToTString(tstr)) {}

	String(const std::wstring& tstr) :
		tstring(ToTString(tstr)) {}

	String(tstring&& str) noexcept :
		tstring(std::move(str)) {}

	String(const tstring& tstr, size_t pos, size_t len = npos) :
		tstring(tstr, pos, len) {}

	String(LPCTSTR s) :
		tstring(s) {}

	String(LPCTSTR s, size_t n) :
		tstring(s, n) {}

	String(size_t n, TCHAR c) :
		tstring(n, c) {}

	template<class InputIterator>
	String(InputIterator first, InputIterator last) :
		tstring(first, last) {}

	String(std::initializer_list<TCHAR> il) :
		tstring(il) {}
};

namespace shoujin::test::string {

TEST_CLASS(StringConversionTest) {
	char const* charValue = "hello";
	wchar_t const* wcharValue = L"hello";
	std::string strValue = "hello";
	std::wstring wstrValue = L"hello";

public:
#ifdef UNICODE
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
