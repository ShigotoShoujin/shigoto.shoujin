#pragma once
namespace shoujin {
template<typename TUserData, typename... TArguments>
class Event {
	using TFunc = void (*)(TArguments..., TUserData&);
	TFunc _func;
	TUserData* _userdata;

public:
	Event() :
		_func{}, _userdata{}
	{}

	Event(TFunc func, TUserData& userdata)
	{
		_func = func;
		_userdata = &userdata;
	}

	Event& operator=(const Event& rhs)
	{
		_func = rhs._func;
		_userdata = rhs._userdata;
		return *this;
	}

	~Event()
	{
		_func = nullptr;
		_userdata = nullptr;
	}

	void operator()(TArguments... args) const
	{
		if(_func)
			_func(args..., *_userdata);
	}

	operator bool() const
	{
		return _func != nullptr;
	}
};
}
