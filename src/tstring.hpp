#ifndef SHOUJIN_SOURCE_TSTRING
#define SHOUJIN_SOURCE_TSTRING

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <sstream>

namespace shoujin {

using tstring = std::basic_string<TCHAR>;
using tstring_view = std::basic_string_view<TCHAR>;
using tstringstream = std::basic_stringstream<TCHAR>;

tstring ToTString(CHAR const* string);
tstring ToTString(WCHAR const* string);
tstring ToTString(std::string const& string);
tstring ToTString(std::wstring const& string);

std::string ToMbString(CHAR const* string);
std::string ToMbString(WCHAR const* string);
std::wstring ToWideString(CHAR const* string);
std::wstring ToWideString(WCHAR const* string);

std::string ToMbString(std::string const& string);
std::string ToMbString(std::wstring const& string);
std::wstring ToWideString(std::string const& string);
std::wstring ToWideString(std::wstring const& string);

int ToInt(tstring_view text);

#ifdef UNICODE
#else
#endif

}

#endif
