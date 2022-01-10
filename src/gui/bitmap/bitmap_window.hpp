#ifndef SHOUJIN_SOURCE_GUI_BITMAP_BITMAP_WINDOW
#define SHOUJIN_SOURCE_GUI_BITMAP_BITMAP_WINDOW

#include "../window.hpp"
#include "bitmap.hpp"
#include <memory>

namespace shoujin::gui::bitmap {

class BitmapWindow : public Window, public Bitmap {

public:
	explicit BitmapWindow(LayoutParam const& = {});

	virtual void BeforeCreate(CreateParam& create_param) override;
	virtual void OnInitialize() override;
	virtual bool OnPaint() override;
	virtual bool OnSizingFinished() override;

	virtual void Repaint();

private:
	virtual Window* Clone() const override;
};

}

#endif
