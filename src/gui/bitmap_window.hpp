#ifndef SHOUJIN_SOURCE_GUI_BITMAP_WINDOW
#define SHOUJIN_SOURCE_GUI_BITMAP_WINDOW

#include "bitmap.hpp"
#include "window.hpp"
#include <memory>

namespace shoujin::gui {

class BitmapWindow : public Window {
	std::unique_ptr<Bitmap> _bitmap;

public:
	explicit BitmapWindow(LayoutParam const& = {});
	BitmapWindow(BitmapWindow const&);
	BitmapWindow& operator=(BitmapWindow const&);
	virtual ~BitmapWindow() = default;

	virtual CreateParam OnCreateParam() override;
	virtual bool OnCreate(CREATESTRUCT const& createparam) override;
	virtual bool OnPaint() override;
	virtual bool OnSizingFinished() override;

private:
	virtual Window* Clone() const override;
};

}

#endif
