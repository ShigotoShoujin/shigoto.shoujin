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

public:
	Event() = default;
	Event(TFunc func, void* userdata = nullptr);
	Event& operator=(Event const& rhs);
	Event& operator=(TFunc func);

	TResult operator()(TArguments... args) const;
	operator bool() const;

	void pushDisable();
	void popDisable();

private:
	TFunc func{};
	void* userdata{};
	int disableDepth{};
};

template<event_tresult TResult, typename... TArguments>
Event<TResult, TArguments...>::Event(TFunc func, void* userdata) :
	func{func},
	userdata{userdata}
{}

template<event_tresult TResult, typename... TArguments>
Event<TResult, TArguments...>
&Event<TResult, TArguments...>::operator=(Event<TResult, TArguments...> const& rhs)
{
	func = rhs.func;
	userdata = rhs.userdata;
	disableDepth = rhs.disableDepth;
	return *this;
}

template<event_tresult TResult, typename... TArguments>
Event<TResult, TArguments...>
&Event<TResult, TArguments...>::operator=(TFunc func)
{
	this->func = func;
	return *this;
}

template<event_tresult TResult, typename... TArguments>
TResult Event<TResult, TArguments...>::operator()(TArguments... args) const
{
	if(operator bool())
		if constexpr(std::is_void_v<TResult>)
			func(args..., userdata);
		else
			return func(args..., userdata);
	else if constexpr(!std::is_void_v<TResult>)
		return TResult{};
}

template<event_tresult TResult, typename... TArguments>
Event<TResult, TArguments...>::operator bool() const
{
	return func != nullptr && disableDepth == 0;
}

template<event_tresult TResult, typename... TArguments>
void Event<TResult, TArguments...>::pushDisable()
{
	++disableDepth;
}

template<event_tresult TResult, typename... TArguments>
void Event<TResult, TArguments...>::popDisable()
{
	if(disableDepth > 0) [[likely]]
		--disableDepth;
}

}

#endif
