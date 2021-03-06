#include "../../assert/assert.hpp"
#include "../window.hpp"
#include "layout_stream.hpp"

namespace shoujin::gui::layout {

// Microsoft Win32 recommended design
// https://docs.microsoft.com/en-us/windows/win32/uxguide/vis-layout#recommended-sizing-and-spacing

constexpr int RelationDialogMargins = 11;
constexpr int RelationBetweenUnrelatedControls = 11;
constexpr int RelationBetweenRelatedControls = 7;
constexpr int RelationDefault = RelationBetweenRelatedControls;

LayoutStream::LayoutStream() :
	_parent{nullptr},
	_padding{RelationDefault}
{}

LayoutStream::LayoutStream(Window* parent) :
	_parent{parent},
	_padding{RelationDefault}
{}

LayoutStream& LayoutStream::operator<<(LayoutStream& (*func)(LayoutStream&))
{
	return func(*this);
}

LayoutStream& LayoutStream::operator<<(LayoutParam const& rhs)
{
	_layout = rhs;
	return *this;
}

LayoutStream& LayoutStream::operator<<(LayoutStream::FromLayout const& rhs)
{
	_from_layout = rhs.layout;
	return *this;
}

LayoutStream& LayoutStream::operator<<(WindowSize const& rhs)
{
	_layout.window_size = rhs.size;
	return *this;
}

LayoutStream& LayoutStream::operator<<(ClientSize const& rhs)
{
	_layout.client_size = rhs.size;
	return *this;
}

LayoutStream& LayoutStream::operator<<(Position const& rhs)
{
	_layout.position = rhs.position;
	return *this;
}

LayoutStream& LayoutStream::operator<<(Style const& rhs)
{
	_layout.style = rhs.style;
	return *this;
}

LayoutStream& LayoutStream::operator<<(ExStyle const& rhs)
{
	_layout.exstyle = rhs.exstyle;
	return *this;
}

LayoutStream& LayoutStream::operator<<(Window* rhs)
{
	SHOUJIN_ASSERT(rhs);
	SHOUJIN_ASSERT(_parent);

	rhs->SetLayout(_layout);
	_parent->AddChild(rhs);
	UpdateLayout(rhs);

	return *this;
}

LayoutStream& LayoutStream::operator<<(CreateParam const& rhs)
{
	auto child = rhs.func(_layout);
	rhs.parent->AddChild(child);
	UpdateLayout(child);
	return *this;
}

LayoutStream& LayoutStream::operator<<(tstring_view const& text)
{
	_layout.text = text;
	return *this;
}

LayoutStream::operator LayoutParam()
{
	return _layout;
}

LayoutStream::FromLayout from(LayoutParam const& rhs)
{
	return LayoutStream::FromLayout{rhs};
}

LayoutStream::WindowSize window_size(Size size)
{
	return LayoutStream::WindowSize{size};
}

LayoutStream::ClientSize client_size(Size size)
{
	return LayoutStream::ClientSize{size};
}

LayoutStream::Position position(Point point)
{
	return LayoutStream::Position{point};
}

LayoutStream::Style style(DWORD rhs)
{
	return LayoutStream::Style{rhs};
}

LayoutStream::ExStyle exstyle(DWORD rhs)
{
	return LayoutStream::ExStyle{rhs};
}

LayoutStream::CreateParam create(Window* parent, LayoutStream::CreateFunc func)
{
	return LayoutStream::CreateParam{.parent = parent, .func = func};
}

LayoutStream& above(LayoutStream& ls)
{
	ls._layout.position = {ls._from_layout.position.x, ls._from_layout.position.y - ls._layout.window_size.y - ls._padding};
	ls._layout.window_size = {ls._layout.window_size};
	return ls;
}

LayoutStream& after(LayoutStream& ls)
{
	ls._layout.position = {ls._from_layout.position.x + ls._from_layout.window_size.x + ls._padding, ls._from_layout.position.y};
	ls._layout.window_size = {ls._layout.window_size};
	return ls;
}

LayoutStream& before(LayoutStream& ls)
{
	ls._layout.position = {ls._from_layout.position.x - ls._layout.window_size.x - ls._padding, ls._from_layout.position.y};
	ls._layout.window_size = {ls._layout.window_size};
	return ls;
}

LayoutStream& below(LayoutStream& ls)
{
	ls._layout.position = {ls._from_layout.position.x, ls._from_layout.position.y + ls._from_layout.window_size.y + ls._padding};
	ls._layout.window_size = {ls._layout.window_size};
	return ls;
}

LayoutStream& related(LayoutStream& ls)
{
	ls._padding = RelationBetweenRelatedControls;
	return ls;
}

LayoutStream& unrelated(LayoutStream& ls)
{
	ls._padding = RelationBetweenUnrelatedControls;
	return ls;
}

LayoutStream& topleft(LayoutStream& ls)
{
	ls._layout.position = {RelationDialogMargins, RelationDialogMargins};
	return ls;
}

LayoutStream& push(LayoutStream& ls)
{
	ls._stack.push(ls._from_layout);
	return ls;
}

LayoutStream& pop(LayoutStream& ls)
{
	if(!ls._stack.empty()) {
		ls._from_layout = ls._stack.top();
		ls._stack.pop();
	}

	return ls;
}

void LayoutStream::UpdateLayout(Window* window)
{
	_from_layout.position = window->position();
	_from_layout.window_size = window->window_size();
	_from_layout.client_size = window->client_size();
	_from_layout.style = window->style();
	_from_layout.exstyle = window->exstyle();
}

}
