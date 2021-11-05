#pragma once
#include "types.hpp"
#include "window_handle.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

class Window : protected WindowHandle {
	Point _position;
	Size _window_size;
	Size _client_size;
	DWORD _style;
	DWORD _exstyle;

public:
	static constexpr DWORD DefaultStyle = WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX;

	enum class CreateMode {
		Default,
		Centered
	};

	struct CreateInfo {
		CreateMode create_mode{};
		Point position{};
		Size window_size = {};
		Size client_size = {};
		DWORD style = {};
		DWORD exstyle = {};
	};

	Window(const CreateInfo& ci = {});
	virtual void Show() override;
	virtual void ShowModal() override;
	virtual void Close() override;

	[[nodiscard]] Point position() const { return _position; }
	[[nodiscard]] Size window_size() const { return _window_size; }
	[[nodiscard]] Size client_size() const { return _client_size; }
	[[nodiscard]] DWORD style() const { return _style; }
	[[nodiscard]] DWORD exstyle() const { return _exstyle; }
};

}
