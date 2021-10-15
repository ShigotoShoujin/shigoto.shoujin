#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../tstring.hpp"

class Window {
	HWND hwnd;
	bool destroyed;

public:
	static const DWORD DEFAULT_STYLE;

	enum class Layout {
		Custom,
		CenterParent,
		FillParent
	};

	struct WindowCreateInfo {
		HWND hwnd_parent{};
		LPCTSTR class_name{};
		LPCTSTR text{};
		Layout layout{};
		POINT position{};
		SIZE client_size{};
		SIZE window_size{};
		DWORD style{};
		DWORD ex_style{};
		HMENU hwnd_menu{};
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

	[[nodiscard]] bool IsDestroyed() const noexcept { return destroyed; }
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
