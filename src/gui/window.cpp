#include "../assert/assert.hpp"
#include "../file/file.hpp"
#include "../file/logfile.hpp"
#include "message_string.hpp"
#include "window.hpp"

using namespace shoujin::gui;

namespace shoujin::gui {

Window::Window(const LayoutInfo& li) :
	WindowCore{li}
{
}

void Window::AddChild(WindowCore* child)
{
	SHOUJIN_ASSERT(child);
	_childs.emplace_back(child);
}

bool Window::ProcessMessages()
{
	SHOUJIN_ASSERT(_hwnd);
	MSG msg;

	while(_hwnd && ReadMessageAsync(msg))
		TranslateAndDispatchMessage(msg);

	return _hwnd;
}

void Window::Close()
{
	SHOUJIN_ASSERT(_hwnd);
	SendMessage(_hwnd, WM_CLOSE, 0, 0);
}

void Window::Show()
{
	if(!_hwnd)
		CreateHandle(*this);

	ProcessMessages();
}

void Window::ShowModal()
{
	if(!_hwnd)
		CreateHandle(*this);

	SHOUJIN_ASSERT(_hwnd);
	MSG msg;
	while(_hwnd && ReadMessage(msg))
		TranslateAndDispatchMessage(msg);
}

void Window::CreateHandle(const Layout& layout, HWND hwnd_parent)
{
	const LPCTSTR CLASS_NAME = TEXT("ShoujinWindow");
	HINSTANCE hinstance = GetModuleHandle(nullptr);
	WNDCLASSEX wc;

	if(!GetClassInfoEx(hinstance, CLASS_NAME, &wc)) {
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProcStatic;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = CLASS_NAME;
		wc.hIconSm = NULL;

		SHOUJIN_ASSERT_WIN32(RegisterClassEx(&wc));
	}

	_hwnd_parent = hwnd_parent;

	DWORD style = layout.style();
	if(_hwnd_parent)
		style |= WS_CHILD;
	else if(!style)
		style = Window::DefaultStyle;

	SHOUJIN_ASSERT(
		CreateWindowEx(
			layout.exstyle(),
			CLASS_NAME,
			CLASS_NAME,
			style,
			layout.position().x,
			layout.position().y,
			layout.window_size().x,
			layout.window_size().y,
			_hwnd_parent,
			nullptr,
			hinstance,
			this));

	SHOUJIN_ASSERT(_hwnd);

	ShowWindow(_hwnd, SW_SHOW);
	SHOUJIN_ASSERT(UpdateWindow(_hwnd));

	for(auto& child : _childs)
		child->CreateHandle(*this, _hwnd);
}

bool Window::ReadMessage(MSG& msg)
{
	auto isok = [](auto result) { return result != -1; }; // GetMessage returns -1 on error
	return SHOUJIN_ASSERT_WIN32_EXPLICIT(GetMessage(&msg, _hwnd, 0, 0), isok);
}

bool Window::ReadMessageAsync(MSG& msg)
{
	return PeekMessage(&msg, _hwnd, 0, 0, PM_REMOVE);
}

void Window::TranslateAndDispatchMessage(const MSG& msg)
{
	TranslateMessage(&msg);
	if(OnDispatchMessage(msg))
		DispatchMessage(&msg);
}

LRESULT CALLBACK Window::WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window* self;

	if(!file::FileExists(file::LogFile::GetDebugFile()))
		file::LogFile::AppendDebug(FormatWindowMessageHeader());

	tstring msg_text = FormatWindowMessageLine(hwnd, msg, wparam, lparam);
	file::LogFile::AppendDebug(msg_text);

	if(msg == WM_NCCREATE) {
		CREATESTRUCT& create_struct = *reinterpret_cast<LPCREATESTRUCT>(lparam);
		self = SHOUJIN_ASSERT(reinterpret_cast<Window*>(create_struct.lpCreateParams));
		self->_hwnd = hwnd;

		SetLastError(0);
		LONG_PTR previous_value = SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
		SHOUJIN_ASSERT_WIN32(previous_value || !GetLastError());
	} else
		self = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if(self)
		if(self->OnWndProc({msg, wparam, lparam}))
			return DefWindowProc(hwnd, msg, wparam, lparam);
		else
			return 0; //The message was already processed by an override and DefWindowProc should not be called

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

}
