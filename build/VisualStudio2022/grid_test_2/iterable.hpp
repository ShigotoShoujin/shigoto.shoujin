#pragma once

template<typename T>
class Iterable
{
public:
	Iterable() :
		_begin{},
		_end{}
	{}

	T* begin() { return _begin; }
	T* end() { return _end; }
	//const T begin() const { return _begin; }
	//const T end() const { return _end; }
	//const T cbegin() const { return _begin; }
	//const T cend() const { return _end; }

	void MoveRange(T* begin)
	{
		_end = begin + (_end - _begin);
		_begin = begin;
	}

private:
	T *_begin, *_end;
};
