#pragma once

namespace shoujin {

template<typename T>
class Iterable
{
public:
	Iterable() :
		_begin{},
		_end{}
	{}

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

	void SetBegin(T* begin)
	{
		_end = begin + (_end - _begin);
		_begin = begin;
	}

	void SetBeginEnd(T* begin, T* end)
	{
		_begin = begin;
		_end = end;
	}

private:
	T *_begin, *_end;
};

}
