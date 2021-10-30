#pragma once

namespace shoujin {
template<typename ...TArguments>
class Event {
	using TFunc = void (*)(TArguments..., void* userdata);
	TFunc _func;
	void* _userdata;

public:
	Event();
	Event(TFunc func, void* userdata = nullptr);
	Event& operator=(const Event& rhs);
	~Event();

	void operator()(TArguments... args) const;
	operator bool() const;
};

template<typename... TArguments>
Event<TArguments...>::Event():
	_func{},
	_userdata{}
{}

template<typename... TArguments>
Event<TArguments...>::Event(TFunc func, void* userdata) :
	_func{func},
	_userdata{userdata}
{}

template<typename... TArguments>
Event<TArguments...>& Event<TArguments...>::operator=(const Event<TArguments...>& rhs)
{
	_func = rhs._func;
	_userdata = rhs._userdata;
	return *this;
}

template<typename... TArguments>
Event<TArguments...>::~Event()
{
	_func = nullptr;
	_userdata = nullptr;
}


template<typename... TArguments>
void Event<TArguments...>::operator()(TArguments... args) const
{
	if(_func)
		_func(args..., _userdata);
}

template<typename... TArguments>
Event<TArguments...>::operator bool() const
{
	return _func != nullptr;
}

}
