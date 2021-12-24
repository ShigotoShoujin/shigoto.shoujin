#ifndef SHOUJIN_SOURCE_GUI_WINDOW
#define SHOUJIN_SOURCE_GUI_WINDOW

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

	explicit Window(LayoutParam const& = {});
	Window(Window const&);
	Window& operator=(Window const&);
	Window(Window&&) = default;
	Window& operator=(Window&&) = default;
	virtual ~Window() = default;

	[[nodiscard]] WindowHandle const* handle() const { return _handle.get(); }
	[[nodiscard]] int const& taborder() const { return _taborder; }

	static Window* FindWindowByHandle(HWND hwnd);

	void AddChild(Window* child);
	Window* GetChild(int index);
	void Close();
	void Destroy();
	bool ProcessMessageQueue();
	void SetFocus();
	void Show();
	void ShowModal();

	Event<bool, MSG const&> OnDispatchMessageEvent;
	Event<bool, WindowMessage const&> OnWndProcEvent;
	Event<bool, Window const&, CREATESTRUCT const&> OnCreateEvent;
	Event<void, Window*> OnInitializeEvent;
	Event<bool> OnCloseEvent;
	Event<bool> OnPaintEvent;
	Event<bool, WPARAM, Rect*> OnSizingEvent;
	Event<bool> OnSizingFinishedEvent;
	Event<> OnDestroyEvent;

protected:
	struct CreateParam {
		LPCTSTR classname;
		bool need_subclassing{};
	};

	void CreateHandle(WindowHandle const* parent = nullptr);
	virtual CreateParam OnCreateParam();

	virtual bool OnDispatchMessage(MSG const& msg);
	virtual bool OnWndProc(WindowMessage const& message);
	virtual bool OnCreate(CREATESTRUCT const& createparam);
	virtual void OnInitialize(Window* source);
	virtual bool OnClose();
	virtual bool OnPaint();
	virtual bool OnSizing(WPARAM wparam, Rect* onsizing_rect);
	virtual bool OnSizingFinished();
	virtual void OnParentSized(Window const& parent);
	virtual void OnDestroy();

private:
	MessageResult RaiseOnDispatchMessage(MSG const& msg);
	MessageResult RaiseOnWndProc(UINT msg, WPARAM wparam, LPARAM lparam);
	MessageResult RaiseOnCreate(WindowMessage const& message);
	void RaiseOnInitialize();
	MessageResult RaiseOnClose();
	MessageResult RaiseOnPaint();
	MessageResult RaiseOnSizing(WindowMessage const& message);
	MessageResult RaiseOnSizingFinished();
	void RaiseOnParentSized();
	MessageResult RaiseOnDestroy();

	virtual Window* Clone() const;
	void CopyChilds(Window const& rhs);
	void ConstructWindow(WindowHandle const* parent);
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK WndProcSubclassStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool ReadMessage(MSG& msg);
	bool ReadMessageAsync(MSG& msg);
	void TranslateAndDispatchMessage(MSG const& msg);
};

}

#endif
