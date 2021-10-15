#pragma once
#include "../class_macro.hpp"
#include "Window.hpp"
#include <cassert>
#include <map>

class Control : public Window {
	std::map<int, Control> child_map;
	int child_map_last_id{};

public:
	DISABLE_COPY(Control)

	Control(Control&& other) noexcept :
		Window{std::move(other)} {}

	Control& operator=(Control&& other) noexcept
	{
		Window::_move(other);
		return *this;
	}

	Control(const Window::WindowCreateInfo& wci) noexcept;
	virtual ~Control() noexcept;

	void AddChild(Control&& control) noexcept;

private:
	void SetParent(const Window& parent_window) noexcept;
};
