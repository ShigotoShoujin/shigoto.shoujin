#pragma once
#include "../event.hpp"
#include "layout.hpp"
#include "window_handle.hpp"
#include <memory>

namespace shoujin::gui {

class Window : public Layout {
	std::unique_ptr<WindowHandle> _handle;

public:
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

	Event<void, const Window&, const CREATESTRUCT&> OnCreateEvent;

protected:
	static const bool kMsgHandled;
	static const bool kMsgNotHandled;

	struct CreateParam {
		LPCTSTR classname;
	};

	void CreateHandle(const WindowHandle* parent = nullptr);

	virtual CreateParam OnCreateParam();
	virtual bool OnDispatchMessage(const MSG& msg);
	virtual bool OnWndProc(const WindowMessage& message);
	virtual bool OnCreate(const CREATESTRUCT& createparam);

private:
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool ReadMessage(MSG& msg);
	bool ReadMessageAsync(MSG& msg);
	void TranslateAndDispatchMessage(const MSG& msg);
};

}
