#ifndef SHOUJIN_SOURCE_GUI_BITMAP_BITMAP_WINDOW
#define SHOUJIN_SOURCE_GUI_BITMAP_BITMAP_WINDOW

#include "../window.hpp"
#include "bitmap.hpp"
#include <memory>

namespace shoujin::gui::bitmap {

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

	[[nodiscard]] virtual Bitmap& bitmap() const { return *_bitmap; }

private:
	virtual Window* Clone() const override;
};

}

#endif
