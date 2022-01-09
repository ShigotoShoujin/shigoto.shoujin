#ifndef SHOUJIN_SOURCE_GUI_ONOFFSWITCH
#define SHOUJIN_SOURCE_GUI_ONOFFSWITCH

namespace shoujin::gui {

/// <summary>
/// Increment the specified reference on construction and decrement on destruction.
/// Used to manage event recursivity.
/// </summary>
class EventDepth {
public:
	EventDepth(int& depth) :
		_depth{depth}
	{
		++_depth;
	}

	~EventDepth()
	{
		if(_depth > 0)
			--_depth;
	}

private:
	int& _depth;
};

}

#endif
