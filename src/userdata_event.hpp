#pragma once
#include "concepts.hpp"

namespace shoujin {
template<typename TUserData, typename... TArguments>
requires concepts::pointer<TUserData>
class UserDataEvent {
	using TFunc = void (*)(TArguments..., TUserData);
	TFunc _func;
	TUserData _userdata;

public:
	UserDataEvent() :
		_func{}, _userdata{}
	{}

	UserDataEvent(TFunc func, TUserData userdata) :
		_func{func}, _userdata{userdata}
	{
	}

	UserDataEvent& operator=(const UserDataEvent& rhs)
	{
		_func = rhs._func;
		_userdata = rhs._userdata;
		return *this;
	}

	~UserDataEvent()
	{
		_func = nullptr;
		_userdata = nullptr;
	}

	void operator()(TArguments... args) const
	{
		if(_func)
			_func(args..., _userdata);
	}

	operator bool() const
	{
		return _func != nullptr;
	}
};
}
