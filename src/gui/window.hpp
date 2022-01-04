#ifndef SHOUJIN_SOURCE_GUI_WINDOW
#define SHOUJIN_SOURCE_GUI_WINDOW

#include "../event.hpp"
#include "layout.hpp"
#include "window_handle.hpp"
#include "window_taborder.hpp"
#include <memory>
#include <vector>

#pragma warning(push)
#pragma warning(disable : 26812)

namespace shoujin::gui {

class Window : public Layout {
	WNDPROC _default_wndproc;
	int _taborder;
	std::unique_ptr<WindowHandle> _handle;
	std::vector<std::unique_ptr<Window>> _child_vec;
	std::unique_ptr<WindowTabOrder> _window_taborder;
	Point _previous_mouse_position;

public:
	static const bool Handled;
	static const bool NotHandled;

	struct WindowMessage {
		UINT msg;
		WPARAM wparam;
		LPARAM lparam;
	};

	struct MessageResult {
		bool handled;
		LRESULT ret_code;
		MessageResult();
		MessageResult(bool handled, LRESULT ret_code = {});
		operator bool() const;
	};

	enum MouseButton : int {
		MouseButtonNone = 0,
		MouseButtonLeft = 1,
		MouseButtonRight = 2,
		MouseButtonMiddle = 4,
		MouseButtonX1 = 8,
		MouseButtonX2 = 16
	};

	struct MouseEvent {
		MouseButton ButtonFlag{};
		Point Position;
		Point Delta;

		MouseEvent() = default;
		explicit MouseEvent(WindowMessage const& message);
	};

	explicit Window(LayoutParam const& = {});
	Window(Window const&);
	Window& operator=(Window const&);
	Window(Window&&) = default;
	Window& operator=(Window&&) = default;
	virtual ~Window() = default;

	[[nodiscard]] WindowHandle const* handle() const { return _handle.get(); }
	[[nodiscard]] HWND hwnd() const { return _handle ? _handle->hwnd() : 0; }
	[[nodiscard]] int const& taborder() const { return _taborder; }

	static Window* FindWindowByHandle(HWND hwnd);

	void AddChild(Window* child);
	Window* GetChild(int index);
	Window* GetChild(HWND hwnd);
	void Close();
	void Destroy();
	bool ProcessMessageQueue();
	void SetFocus();
	void SetText(tstring_view text);
	void Show();
	void ShowModal();
	void Invalidate();

	Event<bool, MSG const&> OnDispatchMessageEvent;
	Event<bool, WindowMessage const&> OnWndProcEvent;
	Event<bool, Window const&, CREATESTRUCT const&> OnCreateEvent;
	Event<void, Window*> OnInitializeEvent;
	Event<bool> OnCloseEvent;
	Event<bool, int> OnCommandEvent;
	Event<bool> OnPaintEvent;
	Event<bool, WPARAM, Rect*> OnSizingEvent;
	Event<bool> OnSizingFinishedEvent;
	Event<bool, Window*, MouseEvent const&> OnMouseDownEvent;
	Event<bool, Window*, MouseEvent const&> OnMouseUpEvent;
	Event<bool, Window*, MouseEvent const&> OnMouseMoveEvent;
	Event<> OnDestroyEvent;

protected:
	struct CreateParam {
		LPCTSTR classname{};
		bool subclass_window{};
	};

	void CreateHandle(WindowHandle const* parent = nullptr);
	virtual void BeforeCreate(CreateParam& create_param);

	virtual bool OnDispatchMessage(MSG const& msg);
	virtual bool OnWndProc(WindowMessage const& message);
	virtual bool OnCreate(CREATESTRUCT const& createparam);
	virtual void OnInitialize(Window* source);
	virtual bool OnClose();
	virtual bool OnCommand(int notification_code);
	virtual bool OnPaint();
	virtual bool OnSizing(WPARAM wparam, Rect* onsizing_rect);
	virtual bool OnSizingFinished();
	virtual void OnParentSized(Window const& parent);
	virtual bool OnMouseDown(MouseEvent const& e);
	virtual bool OnMouseUp(MouseEvent const& e);
	virtual bool OnMouseMove(MouseEvent const& e);
	virtual void OnDestroy();

private:
	MessageResult RaiseOnDispatchMessage(MSG const& msg);
	MessageResult RaiseOnWndProc(UINT msg, WPARAM wparam, LPARAM lparam);
	MessageResult RaiseOnCreate(WindowMessage const& message);
	void RaiseOnInitialize();
	MessageResult RaiseOnClose();
	MessageResult RaiseOnCommand(WindowMessage const& message);
	MessageResult RaiseOnPaint();
	MessageResult RaiseOnSizing(WindowMessage const& message);
	MessageResult RaiseOnSizingFinished();
	void RaiseOnParentSized();
	MessageResult RaiseOnMouseDown(WindowMessage const& message);
	MessageResult RaiseOnMouseUp(WindowMessage const& message);
	MessageResult RaiseOnMouseMove(WindowMessage const& message);
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

#pragma warning(pop)

#endif
