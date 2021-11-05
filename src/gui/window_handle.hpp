#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>

namespace shoujin::gui {

class Window;

class WindowHandle {
	HWND _hwnd;
	HWND _hwnd_parent;

protected:
	struct WindowMessage {
		UINT msg;
		WPARAM wparam;
		LPARAM lparam;
	};

	WindowHandle();
	~WindowHandle();

	void CreateHandle(const Window& window, HWND hwnd_parent);
	void Reset();

	[[nodiscard]] const HWND& hwnd() const { return _hwnd; }
	[[nodiscard]] const HWND& hwnd_parent() const { return _hwnd_parent; }

	virtual bool OnCreate(const CREATESTRUCT& createinfo) { return true; }
	virtual bool OnPaint() { return true; }
	virtual bool OnDispatchMessage(const MSG& msg);
	virtual bool OnWndProc(const WindowMessage& message);
	virtual void ProcessOnPaintMessageFromDC(HDC hsourcedc);
	virtual bool ProcessMessages();
	virtual void ShowModal();
	virtual void Close();

private:
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool ReadMessage(MSG& msg);
	bool ReadMessageAsync(MSG& msg);
	void TranslateAndDispatchMessage(const MSG& msg);
};

}
