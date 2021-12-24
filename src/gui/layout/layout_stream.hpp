#ifndef SHOUJIN_SOURCE_GUI_LAYOUT_LAYOUT_STREAM
#define SHOUJIN_SOURCE_GUI_LAYOUT_LAYOUT_STREAM

#pragma once
#include "../layout.hpp"
#include <stack>

namespace shoujin::gui {
class Window;
}

namespace shoujin::gui::layout {

class LayoutStream {
public:
	using CreateFunc = Window* (*)(LayoutParam const&);

private:
	struct FromLayout {
		const LayoutParam& layout;
	};

	struct WindowSize {
		Size size;
	};

	struct ClientSize {
		Size size;
	};

	struct Style {
		DWORD style;
	};

	struct ExStyle {
		DWORD exstyle;
	};

	struct CreateParam {
		Window* parent;
		CreateFunc func;
		Window** out_ptr;
	};

	struct Position {
		Point position;
	};

public:
	LayoutStream();
	LayoutStream& operator<<(LayoutStream& (*)(LayoutStream&));
	LayoutStream& operator<<(LayoutParam const&);
	LayoutStream& operator<<(FromLayout const&);
	LayoutStream& operator<<(WindowSize const&);
	LayoutStream& operator<<(ClientSize const&);
	LayoutStream& operator<<(Style const&);
	LayoutStream& operator<<(ExStyle const&);
	LayoutStream& operator<<(CreateParam const&);
	LayoutStream& operator<<(Position const&);
	LayoutStream& operator<<(tstring const&);
	operator LayoutParam();

private:
	friend FromLayout from(LayoutParam const&);
	friend WindowSize window_size(Size size);
	friend ClientSize client_size(Size size);
	friend Position position(Point point);
	friend Style style(DWORD style);
	friend ExStyle exstyle(DWORD exstyle);
	friend CreateParam create(Window* parent, CreateFunc func);
	friend LayoutStream& above(LayoutStream&);
	friend LayoutStream& after(LayoutStream&);
	friend LayoutStream& before(LayoutStream&);
	friend LayoutStream& below(LayoutStream&);
	friend LayoutStream& related(LayoutStream&);
	friend LayoutStream& unrelated(LayoutStream&);
	friend LayoutStream& topleft(LayoutStream&);
	friend LayoutStream& push(LayoutStream&);
	friend LayoutStream& pop(LayoutStream&);

	int _padding;
	LayoutParam _layout;
	LayoutParam _from_layout;
	std::stack<LayoutParam> _stack;
};

LayoutStream::FromLayout from(LayoutParam const&);
LayoutStream::WindowSize window_size(Size size);
LayoutStream::ClientSize client_size(Size size);
LayoutStream::Position position(Point point);
LayoutStream::Style style(DWORD style);
LayoutStream::ExStyle exstyle(DWORD exstyle);
LayoutStream::CreateParam create(Window* parent, LayoutStream::CreateFunc func);
LayoutStream& above(LayoutStream&);
LayoutStream& after(LayoutStream&);
LayoutStream& before(LayoutStream&);
LayoutStream& below(LayoutStream&);
LayoutStream& related(LayoutStream&);
LayoutStream& unrelated(LayoutStream&);
LayoutStream& topleft(LayoutStream&);
LayoutStream& push(LayoutStream&);
LayoutStream& pop(LayoutStream&);

}

#endif
