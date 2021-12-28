#include "../../assert/assert.hpp"
#include "../bitmap/bitmap_window.hpp"
#include "../comctl32/edit_control.hpp"
#include "../comctl32/label_control.hpp"
#include "../layout/layout_stream.hpp"
#include "color_control.hpp"

using namespace shoujin::gui;
using namespace shoujin::gui::bitmap;
using namespace shoujin::gui::layout;

namespace shoujin::gui::comctl32 {

Size const ColorControl::kDefaultClientSize{768, 768};

ColorControl::ColorControl(LayoutParam const& lp) :
	Window{BuildLayout(lp)}
{
	auto gradient_map = [](LayoutParam const& lp) -> Window* {
		auto new_lp{lp};
		new_lp.tabstop = false;
		auto bw = new BitmapWindow(new_lp);
		bw->OnInitializeEvent = GradientMap_OnInitialize;
		return bw;
	};

	auto gradient_bar_h = [](LayoutParam const& lp) -> Window* {
		auto new_lp{lp};
		new_lp.tabstop = false;
		auto bw = new BitmapWindow(new_lp);
		bw->OnInitializeEvent = GradientBarH_OnInitialize;
		bw->OnClickEvent = GradientBarH_OnClick;
		return bw;
	};

	auto gradient_bar_v = [](LayoutParam const& lp) -> Window* {
		auto new_lp{lp};
		new_lp.tabstop = false;
		auto bw = new BitmapWindow(new_lp);
		bw->OnInitializeEvent = GradientBarV_OnInitialize;
		return bw;
	};

	auto window = [](LayoutParam const& lp) -> Window* { auto p{lp}; p.tabstop=false; return new Window(p); };
	auto label = [](LayoutParam const& lp) -> Window* { return new LabelControl(lp); };
	auto edit = [](LayoutParam const& lp) -> Window* { return new EditControl(lp); };

	LayoutStream stream;

	stream
		<< layout::window_size(client_size() / 2) << layout::exstyle(WS_EX_CLIENTEDGE)
		<< topleft << create(this, gradient_map)
		<< push << layout::window_size({client_size().x / 2, 23}) << below << create(this, gradient_bar_h) << pop
		<< layout::window_size({23, client_size().y / 2}) << after << create(this, gradient_bar_v)
		<< layout::exstyle(0) << layout::window_size(LabelControl::DefaultSize) << unrelated << after
		<< TEXT("Red") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below
		<< TEXT("Green") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below
		<< TEXT("Blue") << create(this, label) << push << after << TEXT("0") << create(this, edit) << pop << below;

	AddChild(new EditControl(LayoutParam{.anchor{AnchorRight | AnchorBottom}}));
	AddChild(new EditControl(LayoutParam{.anchor{AnchorRight | AnchorTop}}));
	AddChild(new EditControl(LayoutParam{.anchor{AnchorLeft | AnchorBottom}}));
}

Window::CreateParam ColorControl::OnCreateParam()
{
	return CreateParam{.classname = TEXT("ShoujinColorControl")};
}

bool ColorControl::OnCreate(CREATESTRUCT const& createparam)
{
	return Window::OnCreate(createparam);
}

Window* ColorControl::Clone() const
{
	return new ColorControl(*this);
}

LayoutParam ColorControl::BuildLayout(LayoutParam const& lp)
{
	Size client_size;
	if(!lp.window_size && !lp.client_size)
		client_size = kDefaultClientSize;

	LayoutParam layout = lp;
	layout.client_size = client_size;
	layout.exstyle = WS_EX_CLIENTEDGE;

	return layout;
}

void ColorControl::GradientMap_OnInitialize(Window* source, void* userdata)
{
	auto self = static_cast<BitmapWindow*>(source);
	auto& bmp = self->bitmap();
	auto bits = bmp.GetBits();
	bits.RenderGradientMap(Color::White, Color::Red, Color::Black, Color::Black);
	bmp.SetBits(bits);
}

void ColorControl::GradientBarH_OnInitialize(Window* source, void* userdata)
{
	auto self = static_cast<BitmapWindow*>(source);
	auto& bmp = self->bitmap();

	auto bits = bmp.GetBits();
	bits.RenderGradientBarHorizontal();
	bmp.SetBits(bits);
}

void ColorControl::GradientBarH_OnClick(Window* source, Point const& position, void* userdata)
{
	tstringstream tss;

	tss << "X: " << position.x << " Y: " << position.y;
	auto text = tss.str();
	MessageBox(*source->handle(), text.c_str(), L"OnClick", MB_OK | MB_ICONINFORMATION);
}

void ColorControl::GradientBarV_OnInitialize(Window* source, void* userdata)
{
	auto self = static_cast<BitmapWindow*>(source);
	auto& bmp = self->bitmap();

	auto bits = bmp.GetBits();
	bits.RenderGradientBarVertical();
	bmp.SetBits(bits);
}

}
