#pragma once
#include "../event.hpp"
#include "layout.hpp"
#include "window_handle.hpp"
#include <memory>
#include <vector>

namespace shoujin::gui {

class Window : public Layout {
	WNDPROC _default_wndproc;
	std::unique_ptr<WindowHandle> _handle;
	std::vector<std::unique_ptr<Window>> _childs;

public:
	static const bool kMsgHandled;
	static const bool kMsgNotHandled;

	struct WindowMessage {
		UINT msg;
		WPARAM wparam;
		LPARAM lparam;
	};

	explicit Window(const LayoutParam& = {});
	Window(const Window&);
	Window& operator=(const Window&);
	Window(Window&&) = default;
	Window& operator=(Window&&) = default;
	virtual ~Window() = default;

	[[nodiscard]] const WindowHandle* handle() const { return _handle.get(); }
	bool ProcessMessageQueue();
	void Show();
	void ShowModal();
	void AddChild(Window* child);

	Event<bool, const Window&, const CREATESTRUCT&> OnCreateEvent;

protected:
	struct CreateParam {
		LPCTSTR classname;
		bool need_subclassing{};
	};

	void CreateHandle(const WindowHandle* parent = nullptr);

	virtual CreateParam OnCreateParam();
	virtual bool OnDispatchMessage(const MSG& msg);
	virtual bool OnWndProc(const WindowMessage& message);
	virtual bool OnCreate(const CREATESTRUCT& createparam);

private:
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK WndProcSubclassStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool ReadMessage(MSG& msg);
	bool ReadMessageAsync(MSG& msg);
	void TranslateAndDispatchMessage(const MSG& msg);
};

}
