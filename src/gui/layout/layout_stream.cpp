#pragma once
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

LayoutStream& LayoutStream::operator<<(CreateParam const& rhs)
{
	auto child = rhs.func(_layout);
	rhs.parent->AddChild(child);

	_from_layout.position = child->position();
	_from_layout.window_size = child->window_size();
	_from_layout.client_size = child->client_size();
	_from_layout.style = child->style();
	_from_layout.exstyle = child->exstyle();

	return *this;
}

LayoutStream& LayoutStream::operator<<(Point const& rhs)
{
	_layout.position = rhs;
	return *this;
}

LayoutStream& LayoutStream::operator<<(Size const& rhs)
{
	_layout.window_size = rhs;
	return *this;
}
LayoutStream& LayoutStream::operator<<(tstring const& text)
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

}
