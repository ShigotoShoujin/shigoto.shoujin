#include "../assert/assert.hpp"
#include "types.hpp"
#include "window.hpp"

using namespace shoujin::gui;

namespace shoujin::gui {

Window::Window(const LayoutInfo& li) :
	Layout {li}
{
}

void Window::AddChild(Window* child)
{
	SHOUJIN_ASSERT(child);
	_childs.emplace_back(child);
}

bool Window::ProcessMessages()
{
	return WindowHandle::ProcessMessages();
}

void Window::Close()
{
	WindowHandle::Close();
}

void Window::Show()
{
	if(!WindowHandle::hwnd())
		Create();

	WindowHandle::ProcessMessages();
}

void Window::ShowModal()
{
	if(!WindowHandle::hwnd())
		Create();

	WindowHandle::ShowModal();
}

void Window::Create(HWND hwnd_parent)
{
	WindowHandle::CreateHandle(*this, hwnd_parent);
	for(auto& child : _childs)
		child->Create(WindowHandle::hwnd());
}

}
