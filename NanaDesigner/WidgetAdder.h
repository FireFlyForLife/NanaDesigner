#pragma once
#include <nana/gui.hpp>
#include <nana/gui/place.hpp>

#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/widgets/button.hpp>

using namespace nana;

enum WIDGETS {
	BUTTON,
	TEXTBOX,
	LABEL
};

class WidgetAdder
{
protected:
	form& fm;
	place& plc;

public:
	WidgetAdder(form& fm, place& windowToAdd);
	~WidgetAdder();

	template<typename Args>
	widget* createWidget(WIDGETS type, Args args) {
		return 0;
	}

	/*template<typename T>
	void addWidget(std::string name) {
		button btn{ fm, name };
		btn.events().click([](const nana::arg_click& click_arg) {
			T t{ fm };
			plc.field(name) << t;
		});
		fm["toolkit"] << btn;
	}*/
};
