#pragma once
#pragma once
#include "../event.hpp"
#include "layout.hpp"
#include "window_handle.hpp"
#include "window_taborder.hpp"
#include <memory>
#include <vector>

namespace shoujin::gui {

class Window : public Layout {
	WNDPROC _default_wndproc;
	int _taborder;
	std::unique_ptr<WindowHandle> _handle;
	std::vector<std::unique_ptr<Window>> _child_vec;
	std::unique_ptr<WindowTabOrder> _window_group;

public:
	struct WindowMessage {
		UINT msg;
		WPARAM wparam;
		LPARAM lparam;
	};

	struct MessageResult {
		bool handled;
		LRESULT ret_code;

		MessageResult() :
			handled{}, ret_code{} {}

		MessageResult(bool handled, LRESULT ret_code = {}) :
			handled{handled}, ret_code{ret_code} {}

		operator bool() const { return handled; }
	};

	explicit Window(const LayoutParam& = {});
	Window(const Window&);
	Window& operator=(const Window&);
	Window(Window&&) = default;
	Window& operator=(Window&&) = default;
	virtual ~Window() = default;

	[[nodiscard]] const WindowHandle* handle() const { return _handle.get(); }
	[[nodiscard]] const int& taborder() const { return _taborder; }

	static Window* FindWindowByHandle(HWND hwnd);

	void AddChild(Window* child);
	Window* GetChild(int index);
	void Close();
	void Destroy();
	bool ProcessMessageQueue();
	void SetFocus();
	void Show();
	void ShowModal();

	Event<bool, const MSG&> OnDispatchMessageEvent;
	Event<bool, const WindowMessage&> OnWndProcEvent;
	Event<bool, const Window&, const CREATESTRUCT&> OnCreateEvent;
	Event<bool> OnCloseEvent;
	Event<bool> OnPaintEvent;
	Event<bool, WPARAM, Rect*> OnSizingEvent;
	Event<> OnDestroyEvent;

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
	virtual bool OnClose();
	virtual bool OnPaint();
	virtual bool OnSizing(WPARAM wparam, Rect* onsizing_rect);
	virtual void OnParentSized(const Window& parent);
	virtual void OnDestroy();

private:
	MessageResult RaiseOnDispatchMessage(const MSG& msg);
	MessageResult RaiseOnWndProc(UINT msg, WPARAM wparam, LPARAM lparam);
	MessageResult RaiseOnCreate(const WindowMessage& message);
	MessageResult RaiseOnClose();
	MessageResult RaiseOnPaint();
	MessageResult RaiseOnSizing(const WindowMessage& message);
	void RaiseOnParentSized();
	MessageResult RaiseOnDestroy();

	virtual Window* Clone() const;
	void CopyChilds(const Window& rhs);
	void ConstructWindow(const WindowHandle* parent);
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK WndProcSubclassStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool ReadMessage(MSG& msg);
	bool ReadMessageAsync(MSG& msg);
	void TranslateAndDispatchMessage(const MSG& msg);
};

}
