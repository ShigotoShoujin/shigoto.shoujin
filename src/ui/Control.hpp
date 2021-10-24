#pragma once
#include "Window.hpp"
#include <cassert>
#include <memory>

class ControlGroup;

struct UserControlCreateInfo {
	SIZE client_size{};
	int control_id{};
	Window::Layout layout{};
	POINT position{};
	int taborder{};
	bool tabstop{true};
	LPCTSTR text{};
	SIZE window_size{};
};

class Control : public Window {
	std::unique_ptr<ControlGroup> control_group;

	friend ControlGroup;

protected:
	int control_id;
	int taborder;
	bool tabstop;

public:
	Control(Control&&) noexcept;

	struct ControlCreateInfo {
		LPCTSTR class_name{};
		SIZE client_size{};
		int control_id{};
		DWORD ex_style{};
		HWND hwnd_parent{};
		Layout layout{};
		POINT position{};
		DWORD style{};
		int taborder{};
		bool tabstop{true};
		LPCTSTR text{};
		SIZE window_size{};
	};

	Control(const ControlCreateInfo& cci) noexcept;

protected:
	Control(const UserControlCreateInfo& cci, LPCTSTR class_name, DWORD style = {}, DWORD ex_style = {}) noexcept;

public:
	virtual ~Control() noexcept;

	virtual void SetFocus() noexcept;
	Control* AddChild(Control&& control);

	virtual bool BeforeKeyDown(HWND hwnd, WPARAM wparam) noexcept override;

private:
	void SetParent(const Window& parent_window) noexcept;
};
