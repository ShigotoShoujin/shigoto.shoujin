#pragma once
#include "../bitmap.hpp"
#include "../window.hpp"
#include <memory>

namespace shoujin::gui::comctl32 {

class ColorControl : public Window {
	static const Size DefaultClientSize;
	std::unique_ptr<Bitmap> _bitmap;

public:
	ColorControl(const LayoutParam& lp = {});
	ColorControl(const ColorControl&);
	ColorControl& operator=(const ColorControl&);
	virtual ~ColorControl() = default;

	virtual CreateParam OnCreateParam() override;
	virtual bool OnCreate(const CREATESTRUCT& createparam) override;
	virtual void OnDestroy() override;

private:
	virtual Window* Clone() const override;
	static bool OnPaintHandler(void* userdata);
	static LayoutParam BuildLayout(const LayoutParam& lp);
};

}
