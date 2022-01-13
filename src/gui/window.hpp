#ifndef SHOUJIN_SOURCE_GUI_WINDOW
#define SHOUJIN_SOURCE_GUI_WINDOW

#include "../enum_operators.hpp"
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

	enum class MouseButton : int {
		None = 0,
		Left = 1,
		Right = 2,
		Middle = 4,
		X1 = 8,
		X2 = 16
	};

	SHOUJIN_DEFINE_ENUM_FLAG_OPERATORS_FRIEND(MouseButton)

	struct KeyEvent {
		uint8_t virtual_keycode;

		KeyEvent() = default;
		explicit KeyEvent(WindowMessage const& message);
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
	[[nodiscard]] tstring GetText() const;
	void AppendText(tstring_view text);
	void AppendLine(tstring_view text);
	void SetText(tstring_view text);
	void Show();
	void ShowModal();
	void Invalidate();

	Event<bool, MSG const&> OnDispatchMessageEvent;
	Event<bool, WindowMessage const&> OnWndProcEvent;
	Event<bool, Window const&, CREATESTRUCT const&> OnCreateEvent;
	Event<void, Window*> OnInitializeEvent;
	Event<void, Window*> OnReadyEvent;
	Event<bool> OnCloseEvent;
	Event<bool, int> OnCommandEvent;
	Event<bool> OnPaintEvent;
	Event<bool, WPARAM, Rect*> OnSizingEvent;
	Event<bool> OnSizingFinishedEvent;
	Event<bool, Window*, KeyEvent const&> OnKeyDownEvent;
	Event<bool, Window*, KeyEvent const&> OnKeyUpEvent;
	Event<bool, Window*, KeyEvent const&> OnKeyPressEvent;
	Event<bool, Window*, MouseEvent const&> OnMouseDownEvent;
	Event<bool, Window*, MouseEvent const&> OnMouseUpEvent;
	Event<bool, Window*, MouseEvent const&> OnMouseClickEvent;
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
	virtual void OnInitialize();
	virtual void OnReady();
	virtual bool OnClose();
	virtual bool OnCommand(int notification_code);
	virtual bool OnPaint();
	virtual bool OnSizing(WPARAM wparam, Rect* onsizing_rect);
	virtual bool OnSizingFinished();
	virtual void OnParentSized(Window const& parent);
	virtual bool OnKeyDown(KeyEvent const& e);
	virtual bool OnKeyUp(KeyEvent const& e);
	virtual bool OnKeyPress(KeyEvent const& e);
	virtual bool OnMouseDown(MouseEvent const& e);
	virtual bool OnMouseUp(MouseEvent const& e);
	virtual bool OnMouseMove(MouseEvent const& e);
	virtual bool OnMouseClick(MouseEvent const& e);
	virtual void OnDestroy();

private:
	MessageResult RaiseOnDispatchMessage(MSG const& msg);
	MessageResult RaiseOnWndProc(UINT msg, WPARAM wparam, LPARAM lparam);
	MessageResult RaiseOnCreate(WindowMessage const& message);
	void RaiseOnInitialize();
	void RaiseOnReady();
	MessageResult RaiseOnClose();
	MessageResult RaiseOnCommand(WindowMessage const& message);
	MessageResult RaiseOnPaint();
	MessageResult RaiseOnSizing(WindowMessage const& message);
	MessageResult RaiseOnSizingFinished();
	void RaiseOnParentSized();
	MessageResult RaiseOnKeyDown(WindowMessage const& message);
	MessageResult RaiseOnKeyUp(WindowMessage const& message);
	MessageResult RaiseOnKeyPress(WindowMessage const& message);
	MessageResult RaiseOnMouseDown(WindowMessage const& message);
	MessageResult RaiseOnMouseUp(WindowMessage const& message);
	MessageResult RaiseOnMouseClick(WindowMessage const& message);
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
	std::vector<TCHAR> GetTextVector(size_t extra_char_to_alloc = 0) const;
	void AppendText(tstring_view text, bool append_new_line);
};

}

#endif
