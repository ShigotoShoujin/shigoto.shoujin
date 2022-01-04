#include "tstring.hpp"
#include <tchar.h>
#include <vector>

namespace shoujin {

#ifdef UNICODE
//This line will stop clang-format from malformating the function following a preprocessor instruction

tstring ToTString(CHAR const* string) { return ToWideString(string); }
tstring ToTString(WCHAR const* string) { return string; }
tstring ToTString(std::string const& string) { return ToTString(string.c_str()); }
tstring ToTString(std::wstring const& string) { return string; }

#else
//This line will stop clang-format from malformating the function following a preprocessor instruction

tstring ToTString(CHAR const* string) { return string; }
tstring ToTString(WCHAR const* string) { return ToMbString(string); }
tstring ToTString(std::string const& string) { return string; }
tstring ToTString(std::wstring const& string) { return ToTString(string.c_str()); }

#endif

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

int ToInt(tstring_view text)
{
	return _tcstol(text.data(), 0, 10);
}

}
