#pragma once

namespace shoujin {
template<typename... TArguments>
class Event {
	using TFunc = void (*)(TArguments...);
	TFunc _func;

public:
	Event() :
		_func{}
	{}

	Event(TFunc func) :
		_func{func}
	{
	}

	Event& operator=(const Event& rhs)
	{
		_func = rhs._func;
		return *this;
	}

	~Event()
	{
		_func = nullptr;
	}

	void operator()(TArguments... args) const
	{
		if(_func)
			_func(args...);
	}

	operator bool() const
	{
		return _func != nullptr;
	}
};
}
