#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>

namespace shoujin::gui {

class Window;

class WindowHandle {
	using ParentHandle = std::shared_ptr<WindowHandle>;
	HWND _hwnd;
	ParentHandle _parent;

protected:
	struct WindowMessage {
		UINT msg;
		WPARAM wparam;
		LPARAM lparam;
	};

	WindowHandle();
	~WindowHandle();

	void CreateHandle(const Window& window);
	void Reset();

	[[nodiscard]] HWND hwnd() const { return _hwnd; }
	[[nodiscard]] ParentHandle const parent() const { return _parent; }

	virtual bool OnDispatchMessage(const MSG& msg);
	virtual bool OnWndProc(const WindowMessage& message);
	virtual void ProcessOnPaintMessageFromDC(HDC hsourcedc);
	virtual void Show();
	virtual void ShowModal();
	virtual void Close();

private:
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool ReadMessage(MSG& msg);
	bool ReadMessageAsync(MSG& msg);
	void TranslateAndDispatchMessage(const MSG& msg);
};

}
