#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../tstring.hpp"

class Window {
	static const TCHAR* DEFAULT_CLASS; //Using TCHAR* because LPCTSTR provoke warning C26495 in VS 17 preview 5.
	static const SIZE DEFAULT_SIZE;

	HWND hwnd;
	bool destroyed;

	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;

protected:
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) noexcept;

public:
	static const DWORD DEFAULT_STYLE;

	enum class Layout {
		Custom,
		CenterParent,
		FillParent
	};

	struct WindowCreateInfo {
		HWND hwnd_parent{};
		LPCTSTR classname{DEFAULT_CLASS};
		LPCTSTR text{};
		Layout layout{};
		POINT position{CW_USEDEFAULT, CW_USEDEFAULT};
		SIZE client_size{};
		SIZE window_size{DEFAULT_SIZE};
		DWORD style{DEFAULT_STYLE};
		DWORD ex_style{};
		HMENU hwnd_menu{};
	};

	Window(WindowCreateInfo wci) noexcept;

	[[nodiscard]] bool IsDestroyed() const noexcept { return destroyed; }

	virtual void Show() noexcept;
	virtual void Hide() noexcept;
	virtual bool MessageUpdate() noexcept;
	virtual bool MessageLoop() noexcept;
	virtual void Destroy() noexcept;

	virtual bool OnMouseClick(WPARAM wparam, int x, int y) noexcept;
	virtual bool BeforeKeyDown(HWND hwnd, WPARAM wparam) noexcept;
	virtual bool OnKeyDown(WPARAM wparam) noexcept;

	virtual ~Window() noexcept;

	[[nodiscard]] virtual HWND GetHandle() const noexcept { return hwnd; }
	[[nodiscard]] virtual SIZE GetWindowSize() const noexcept;
	[[nodiscard]] virtual SIZE GetClientSize() const noexcept;
	[[nodiscard]] virtual tstring GetText() const noexcept;

private:
	void ProcessMessage(const MSG& msg) noexcept;
};

extern inline SIZE RectToSize(const RECT& rect);
