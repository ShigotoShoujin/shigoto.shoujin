#ifndef SHOUJIN_SOURCE_GUI_COMCTL32
#define SHOUJIN_SOURCE_GUI_COMCTL32

#include "../window.hpp"

namespace shoujin::gui::comctl32 {

class Comctl32 : public Window {
protected:
	explicit Comctl32(LPCTSTR classname, LayoutParam const&);

public:
	virtual ~Comctl32() = default;
	virtual void BeforeCreate(CreateParam& create_param) final override;

private:
	LPCTSTR _classname;
};

}

#endif
