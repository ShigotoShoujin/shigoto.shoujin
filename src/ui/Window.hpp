#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../class_macro.hpp"
#include "../tstring.hpp"

class Window {
protected:
	bool active;
	HWND hwnd;
	SIZE window_size;
	DWORD style;

public:
	DISABLE_COPY(Window)
	ENABLE_MOVE_CONSTRUCTOR(Window)

	static const DWORD DEFAULT_STYLE;

	enum class Layout {
		Custom,
		CenterParent,
		FillParent
	};

	struct WindowCreateInfo {
		LPCTSTR class_name{};
		SIZE client_size{};
		DWORD ex_style{};
		HMENU hwnd_menu{};
		HWND hwnd_parent{};
		Layout layout{};
		POINT position{};
		DWORD style{};
		LPCTSTR text{};
		SIZE window_size{};
	};

	Window(const WindowCreateInfo& wci) noexcept;
	virtual ~Window() noexcept;

	virtual void Show() noexcept;
	virtual void Hide() noexcept;
	virtual bool MessageUpdate() noexcept;
	virtual bool MessageLoop() noexcept;
	virtual void Destroy() noexcept;

	virtual bool OnMouseClick(WPARAM wparam, int x, int y) noexcept;
	virtual bool BeforeKeyDown(HWND hwnd, WPARAM wparam) noexcept;
	virtual bool OnKeyDown(WPARAM wparam) noexcept;

	[[nodiscard]] bool IsDestroyed() const noexcept { return !active; }
	[[nodiscard]] virtual HWND GetHandle() const noexcept { return hwnd; }
	[[nodiscard]] virtual SIZE GetWindowSize() const noexcept;
	[[nodiscard]] virtual SIZE GetClientSize() const noexcept;
	[[nodiscard]] virtual tstring GetText() const noexcept;

protected:
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) noexcept;

private:
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
	void PrepareWndClass(HINSTANCE hinstance, LPCTSTR class_name) const noexcept;
	void ProcessMessage(const MSG& msg) noexcept;
};

inline SIZE RectToSize(const RECT& rect) noexcept;
