#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <sstream>

namespace shoujin {
using tstring = std::basic_string<TCHAR>;
using tstringstream = std::basic_stringstream<TCHAR>;
}
