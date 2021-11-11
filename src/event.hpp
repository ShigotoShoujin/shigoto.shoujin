#pragma once
#include "concepts.hpp"

// clang does not yet support c++20 concepts
// clang-format off

namespace shoujin {

template<typename TResult = void, typename... TArguments> requires fundamental<TResult>
class Event {
	using TFunc = TResult (*)(TArguments..., void* userdata);
	TFunc _func;
	void* _userdata;

public:
	Event();
	Event(TFunc func, void* userdata = nullptr);
	Event& operator=(const Event& rhs);
	~Event();

	TResult operator()(TArguments... args) const;
	operator bool() const;
};

template<typename TResult, typename... TArguments> requires fundamental<TResult>
Event<TResult, TArguments...>::Event() :
	_func{},
	_userdata{}
{}

template<typename TResult, typename... TArguments> requires fundamental<TResult>
Event<TResult, TArguments...>::Event(TFunc func, void* userdata) :
	_func{func},
	_userdata{userdata}
{}

template<typename TResult, typename... TArguments> requires fundamental<TResult>
Event<TResult, TArguments...>
&Event<TResult, TArguments...>::operator=(const Event<TResult, TArguments...>& rhs)
{
	_func = rhs._func;
	_userdata = rhs._userdata;
	return *this;
}

template<typename TResult, typename... TArguments> requires fundamental<TResult>
Event<TResult, TArguments...>::~Event()
{
	_func = nullptr;
	_userdata = nullptr;
}

template<typename TResult, typename... TArguments> requires fundamental<TResult>
TResult Event<TResult, TArguments...>::operator()(TArguments... args) const
{
	if(_func)
		if constexpr(std::is_void_v<TResult>)
			_func(args..., _userdata);
		else
			return _func(args..., _userdata);
	else if constexpr(!std::is_void_v<TResult>)
		return TResult{};
}

template<typename TResult, typename... TArguments> requires fundamental<TResult>
Event<TResult, TArguments...>::operator bool() const
{
	return _func != nullptr;
}

}
