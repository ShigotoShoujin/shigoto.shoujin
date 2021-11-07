#pragma once
#include "../bitmap.hpp"
#include "../types.hpp"
#include "../window.hpp"
#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

class ColorPicker : public Window {
	std::unique_ptr<Bitmap> _bitmap;

public:
	ColorPicker(Point position, Size window_size);

	ColorPicker(const ColorPicker&) = delete;
	ColorPicker& operator=(const ColorPicker&) = delete;
	ColorPicker(ColorPicker&&) noexcept = default;
	ColorPicker& operator=(ColorPicker&&) noexcept = default;

	virtual bool OnCreate(const CREATESTRUCT& createinfo) override;
	virtual bool OnPaint() override;
};

}
