module;

export module ScopeGuard;

namespace shoujin {

export template<typename UserDataT>
class ScopeGuard {
public:
	using CleanupFunc = void (*)(UserDataT*);
	explicit ScopeGuard(CleanupFunc func, UserDataT* userdata = nullptr);
	~ScopeGuard();
	void release();

private:
	bool engaged;
	CleanupFunc func;
	UserDataT* userdata;
};

template<typename UserDataT>
ScopeGuard<UserDataT>::ScopeGuard(CleanupFunc func, UserDataT* userdata) :
	engaged{true},
	func{func},
	userdata{userdata}
{}

template<typename UserDataT>
ScopeGuard<UserDataT>::~ScopeGuard()
{
	if(engaged && func)
		func(userdata);
}

template<typename UserDataT>
void ScopeGuard<UserDataT>::release()
{
	engaged = false;
}

}
