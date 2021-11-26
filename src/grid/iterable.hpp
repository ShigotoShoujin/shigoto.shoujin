#pragma once

namespace shoujin {

template<typename T>
class Iterable
{
public:
	Iterable(T* begin, T* end) :
		_begin{begin},
		_end{end}
	{}

	[[nodiscard]] T* begin() { return _begin; }
	[[nodiscard]] T* end() { return _end; }
	[[nodiscard]] const T* begin() const { return _begin; }
	[[nodiscard]] const T* end() const { return _end; }
	[[nodiscard]] const T* cbegin() const { return _begin; }
	[[nodiscard]] const T* cend() const { return _end; }

	void MoveRange(T* begin)
	{
		_end = begin + (_end - _begin);
		_begin = begin;
	}

private:
	T *_begin, *_end;
};

}
