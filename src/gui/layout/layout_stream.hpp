#pragma once
#include "../layout.hpp"
#include <stack>

namespace shoujin::gui {
class Window;
}

namespace shoujin::gui::layout {

class LayoutStream {
public:
	using CreateFunc = Window* (*)(const LayoutParam&);

private:
	struct FromLayout {
		const LayoutParam& layout;
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
	};

public:
	using CreateFunc = Window* (*)(const LayoutParam&);
	LayoutStream();
	LayoutStream& operator<<(LayoutStream& (*)(LayoutStream&));
	LayoutStream& operator<<(const LayoutParam&);
	LayoutStream& operator<<(const FromLayout&);
	LayoutStream& operator<<(const Style&);
	LayoutStream& operator<<(const ExStyle&);
	LayoutStream& operator<<(const CreateParam&);
	LayoutStream& operator<<(const Point&);
	LayoutStream& operator<<(const Size&);
	LayoutStream& operator<<(const tstring&);
	operator LayoutParam();

private:
	friend FromLayout from(const LayoutParam&);
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

LayoutStream::FromLayout from(const LayoutParam&);
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
