#include "color_picker.h"

namespace nana {
	arg_color_picker::arg_color_picker(color_picker& cp)
		: widget(cp)
	{
	}

	namespace drawerbase
	{
		namespace color_picker
		{
			
		}
	}

	drawerbase::color_picker::drawer::drawer()
	{
	}

	color_picker::color_picker()
	{
	}

	color_picker::color_picker(window wd, bool visible)
	{
		this->create(wd, visible);
	}

	color_picker::color_picker(window wd, const nana::rectangle& r, bool visible)
	{
		this->create(wd, r, visible);
	}
}
