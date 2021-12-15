#ifndef SHOUJIN_SOURCE_GUI_BITMAP_BITS
#define SHOUJIN_SOURCE_GUI_BITMAP_BITS

#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace shoujin::gui {

class BitmapBits {
public:
	struct Pixel {
		uint8_t b, g, r;
	};

	struct PixelRow {
		Pixel *_begin, *_end;

		PixelRow(Pixel* begin, int width) :
			_begin{begin}, _end{begin + width} {}

		[[nodiscard]] Pixel* begin() { return _begin; }
		[[nodiscard]] Pixel* end() { return _end; }
	};

	struct PixelRowsIterator {
		struct Iterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Pixel;
			using pointer = value_type*;
			using reference = value_type&;

			Iterator(pointer begin, int width) :
				_at(begin), _width{width} {}

			reference operator*() const { return *_at; }
			pointer operator->() { return _at; }

			Iterator& operator++()
			{
				_at += _width;
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator self = *this;
				_at += _width;
				return self;
			}

			friend bool operator==(const Iterator& a, const Iterator& b) { return a._at == b._at; };
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a._at != b._at; };

		private:
			pointer _at;
			int _width;
		};

		PixelRowsIterator(Pixel* begin, int width) :
			_begin{begin, width}, _end{begin, width} {}

		[[nodiscard]] Iterator begin() const { return _begin; }
		[[nodiscard]] Iterator end() const { return _end; }

	private:
		Iterator _begin, _end;
	};

private:
	int _width, _height;
	std::unique_ptr<Pixel[]> _pixels;
	Pixel* _end;

public:
	BitmapBits(int width, int height);
	[[nodiscard]] int width() const { return _width; }
	[[nodiscard]] int height() const { return _height; }
	[[nodiscard]] Pixel* begin() const { return _pixels.get(); }
	[[nodiscard]] Pixel* end() const { return _end; }
	[[nodiscard]] PixelRowsIterator PixelRows() { return PixelRowsIterator{_pixels.get(), _width}; }
};

}

#endif
