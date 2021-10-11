#pragma once
#include "Window.hpp"

class Control : public Window {
public:
	static const int AUTO;

	struct Args {
		int id{};
		HWND hwnd_parent{};
		LPCTSTR text{};
		Window::Mode mode{};
		POINT position{};
		SIZE size{};
		DWORD style;
		DWORD ex_style;
		int taborder{Control::AUTO};
		bool tabstop{true};
	};

protected:
	Control(Args args, LPCTSTR classname) noexcept;

public:
	virtual ~Control() override {}

	virtual void SetFocus() noexcept;

	[[nodiscard]] bool IsAutoTabOrder() const noexcept { return tabstop && taborder == AUTO; }
	[[nodiscard]] int GetTabOrder() const noexcept { return taborder; };
	[[nodiscard]] bool IsTabStop() const noexcept { return tabstop; };

	void SetTabOrder(int taborder) noexcept { Control::taborder = taborder; }

private:
	int id;
	int taborder;
	bool tabstop;
};
