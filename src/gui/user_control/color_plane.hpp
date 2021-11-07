#pragma once
#include "../bitmap.hpp"
#include "../types.hpp"
#include "../window.hpp"
#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

class ColorPlane : public Window {
	std::unique_ptr<Bitmap> _bitmap;

public:
	ColorPlane(Point position, Size window_size);

	ColorPlane(const ColorPlane&) = delete;
	ColorPlane& operator=(const ColorPlane&) = delete;
	ColorPlane(ColorPlane&&) noexcept = default;
	ColorPlane& operator=(ColorPlane&&) noexcept = default;

	virtual bool OnCreate(const CREATESTRUCT& createinfo) override;
	virtual bool OnPaint() override;
};

}
