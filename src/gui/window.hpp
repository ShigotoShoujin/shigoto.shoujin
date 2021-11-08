#pragma once
#include "layout.hpp"
#include "types.hpp"
#define WIN32_LEAN_AND_MEAN
#include "window_handle.hpp"
#include <vector>
#include <Windows.h>

namespace shoujin::gui {

class Window : protected WindowHandle, public Layout {
	std::vector<std::unique_ptr<Window>> _childs;

public:
	Window(const LayoutInfo& li = {});

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) noexcept = default;
	Window& operator=(Window&&) noexcept = default;

	virtual void AddChild(Window* child);
	virtual bool ProcessMessages() override;
	virtual void Close() override;
	virtual void Show();
	virtual void ShowModal() override;

private:
	void Create(HWND hwnd_parent = nullptr);
};

}
