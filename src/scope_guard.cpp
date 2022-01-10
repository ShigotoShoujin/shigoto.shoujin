module;

export module ScopeGuard;

namespace shoujin {

export template<typename UserDataT>
class ScopeGuard {
public:
	explicit ScopeGuard(void (*)(UserDataT*), UserDataT* = nullptr);
	~ScopeGuard();
	void release();

private:
	bool engaged;
	void (*func)(UserDataT*);
	UserDataT* userdata;
};

template<typename UserDataT>
ScopeGuard<UserDataT>::ScopeGuard(void (*func)(UserDataT*), UserDataT* userdata) :
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
