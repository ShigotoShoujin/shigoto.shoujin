module;
#include <sstream>
#include <string>
#include <tchar.h>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module Shoujin.String;

export namespace shoujin {

using tstring = std::basic_string<TCHAR>;
using tstring_view = std::basic_string_view<TCHAR>;
using tstringstream = std::basic_stringstream<TCHAR>;

using StringView = tstring_view;

namespace string {

std::string ToMbString(CHAR const* string)
{
	return string;
}

std::string ToMbString(WCHAR const* string)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, string, -1, nullptr, 0, 0, nullptr);
	std::vector<CHAR> buffer(size);
	WideCharToMultiByte(CP_UTF8, 0, string, -1, buffer.data(), size, 0, nullptr);
	return std::string(buffer);
}

std::wstring ToWideString(CHAR const* string)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, string, -1, nullptr, 0);
	std::vector<WCHAR> buffer(size);
	MultiByteToWideChar(CP_UTF8, 0, string, -1, buffer.data(), size);
	return std::wstring(buffer);
}

std::wstring ToWideString(WCHAR const* string)
{
	return string;
}

std::string ToMbString(std::string const& string)
{
	return string;
}

std::string ToMbString(std::wstring const& string)
{
	return ToMbString(string.c_str());
}

std::wstring ToWideString(std::string const& string)
{
	return ToWideString(string.c_str());
}

std::wstring ToWideString(std::wstring const& string)
{
	return string;
}

// clang-format off
#ifdef UNICODE

tstring ToTString(CHAR const* string) { return ToWideString(string); }
tstring ToTString(WCHAR const* string) { return string; }
tstring ToTString(std::string const& string) { return ToTString(string.c_str()); }
tstring ToTString(std::wstring const& string) { return string; }

#else

tstring ToTString(CHAR const* string) { return string; }
tstring ToTString(WCHAR const* string) { return ToMbString(string); }
tstring ToTString(std::string const& string) { return string; }
tstring ToTString(std::wstring const& string) { return ToTString(string.c_str()); }

#endif
// clang-format on

int ToNumeric(tstring_view text)
{
	return _tcstol(text.data(), 0, 10);
}

}

using namespace string;

class String : public tstring {
public:
	String() :
		tstring() {}

	String(const std::string& tstr) :
		tstring(ToTString(tstr)) {}

	String(const std::wstring& tstr) :
		tstring(ToTString(tstr)) {}

	String(tstring&& str) noexcept :
		tstring(std::move(str)) {}

	String(const tstring& tstr, size_t pos, size_t len = npos) :
		tstring(tstr, pos, len) {}

	String(LPCSTR s) :
		tstring(ToTString(s)) {}

	String(LPCWSTR s) :
		tstring(ToTString(s)) {}

	String(StringView const& sv) :
		tstring(sv.data(), sv.size()) {}
};

}
