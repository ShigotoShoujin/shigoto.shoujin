#pragma once
#include "../class_macro.hpp"
#include "Window.hpp"
#include <cassert>

class ControlGroup;

class Control : public Window {
	Control& _move(Control& other) noexcept;
	ControlGroup* control_group;

	friend ControlGroup;

protected:
	int control_id{};
	int taborder{0};
	bool tabstop{true};

public:
	ENABLE_MOVE(Control)

	Control(const Window::WindowCreateInfo& wci) noexcept;
	virtual ~Control() noexcept;

	virtual void SetFocus() noexcept;
	void AddChild(Control&& control) noexcept;

	virtual bool BeforeKeyDown(HWND hwnd, WPARAM wparam) noexcept override;

private:
	void SetParent(const Window& parent_window) noexcept;
};
