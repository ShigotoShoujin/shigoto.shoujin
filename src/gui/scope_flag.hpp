#ifndef SHOUJIN_SOURCE_GUI_ONOFFSWITCH
#define SHOUJIN_SOURCE_GUI_ONOFFSWITCH

namespace shoujin::gui {

/// <summary>
/// Invert the specified bool reference when going out of scope.
/// Used to ensure that a flag always gets reset.
/// </summary>
class ScopeFlag {
public:
	ScopeFlag(bool& state) :
		_state{state} {}

	ScopeFlag(bool& state, bool initial_state) :
		_state{state}
	{
		_state = initial_state;
	}

	ScopeFlag& operator=(bool state)
	{
		_state = state;
		return *this;
	}

	inline operator bool() const
	{
		return _state;
	}

	~ScopeFlag()
	{
		_state = !_state;
	}

private:
	bool& _state;
};

}

#endif
