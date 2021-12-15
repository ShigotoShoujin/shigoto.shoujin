#ifndef SHOUJIN_SOURCE_EVENT
#define SHOUJIN_SOURCE_EVENT

#include <type_traits>

// clang does not yet support c++20 concepts
// clang-format off

namespace shoujin {

template<typename T> concept event_tresult = std::is_void_v<T> || std::is_default_constructible_v<T>;

template<event_tresult TResult = void, typename... TArguments>
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

template<event_tresult TResult, typename... TArguments>
Event<TResult, TArguments...>::Event() :
	_func{},
	_userdata{}
{}

template<event_tresult TResult, typename... TArguments>
Event<TResult, TArguments...>::Event(TFunc func, void* userdata) :
	_func{func},
	_userdata{userdata}
{}

template<event_tresult TResult, typename... TArguments>
Event<TResult, TArguments...>
&Event<TResult, TArguments...>::operator=(const Event<TResult, TArguments...>& rhs)
{
	_func = rhs._func;
	_userdata = rhs._userdata;
	return *this;
}

template<event_tresult TResult, typename... TArguments>
Event<TResult, TArguments...>::~Event()
{
	_func = nullptr;
	_userdata = nullptr;
}

template<event_tresult TResult, typename... TArguments>
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

template<event_tresult TResult, typename... TArguments>
Event<TResult, TArguments...>::operator bool() const
{
	return _func != nullptr;
}

}

#endif
