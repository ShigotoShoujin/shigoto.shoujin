#pragma once
#include "window_core.hpp"
#include <memory>
#include <vector>

namespace shoujin::gui {

class Window : public WindowCore {
	HWND _hwnd;
	HWND _hwnd_parent;
	std::vector<std::unique_ptr<WindowCore>> _childs;

public:
	Window(const LayoutInfo& li = {});

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) noexcept = default;
	Window& operator=(Window&&) noexcept = default;

	virtual void AddChild(WindowCore* child);
	virtual bool ProcessMessages();
	virtual void Close();
	virtual void Show();
	virtual void ShowModal();

private:
	virtual void CreateHandle(const Layout& layout, HWND hwnd_parent = nullptr) final override;
	bool ReadMessage(MSG& msg);
	bool ReadMessageAsync(MSG& msg);
	void TranslateAndDispatchMessage(const MSG& msg);
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

}
