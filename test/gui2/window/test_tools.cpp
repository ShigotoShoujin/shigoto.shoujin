#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::test::gui2::test_tools {

void getCenteredWindowPosition(int size_x, int size_y, int& out_x, int& out_y)
{
	out_x = (GetSystemMetrics(SM_CXSCREEN) + size_x) >> 1;
	out_y = (GetSystemMetrics(SM_CYSCREEN) + size_x) >> 1;
}

}
