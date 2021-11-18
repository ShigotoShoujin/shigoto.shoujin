#pragma once
#include "bitmap.hpp"
#include "window.hpp"
#include <memory>

namespace shoujin::gui {

class BitmapWindow : public Window {
	std::unique_ptr<Bitmap> _bitmap;

public:
	explicit BitmapWindow(const LayoutParam& = {});
	BitmapWindow(const BitmapWindow&);
	BitmapWindow& operator=(const BitmapWindow&);
	virtual ~BitmapWindow() = default;

	virtual CreateParam OnCreateParam() override;
	virtual bool OnCreate(const CREATESTRUCT& createparam) override;
	virtual bool OnPaint() override;
	virtual bool OnSizingFinished() override;

private:
	virtual Window* Clone() const override;
};

}
