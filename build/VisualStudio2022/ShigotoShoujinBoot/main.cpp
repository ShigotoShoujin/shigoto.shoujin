#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void boot();

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{
	return boot(), 0;
}
