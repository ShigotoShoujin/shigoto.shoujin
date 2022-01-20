#ifndef SHOUJIN_TEST_GUI_UTIL
#define SHOUJIN_TEST_GUI_UTIL

import Shoujin.Gui.Window;
import Shoujin.String;

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace shoujin;
using namespace shoujin::gui2;

namespace Microsoft::VisualStudio::CppUnitTestFramework {

template<> inline std::wstring ToString<Point>(Point const& t)
{
	std::wstringstream wss;
	wss << L"Point(" << t.x << L',' << t.y << L')';
	return wss.str();
}

template<> inline std::wstring ToString<Size>(Size const& t)
{
	std::wstringstream wss;
	wss << L"Point(" << t.x << L',' << t.y << L')';
	return wss.str();
}

template<> inline std::wstring ToString<WindowStyle>(WindowStyle const& t)
{
	std::wstringstream wss;
	wss << L"WindowStyle = " << static_cast<int>(t);
	return wss.str();
}

template<> inline std::wstring ToString<String>(String const& t)
{
	return string::ToWideString(t);
}

}

#endif
