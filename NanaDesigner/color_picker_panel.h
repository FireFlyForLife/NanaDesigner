#pragma once
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/drawing.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/spinbox.hpp>

using namespace nana;

class color_picker_panel : public nana::panel<true>
{
public:
	struct arg_color_picker : event_arg
	{
		color_picker_panel& color_picker;
		nana::color color;

		arg_color_picker(color_picker_panel& cpc, nana::color color);
	};

	basic_event<arg_color_picker> color_changed;

public:
	color_picker_panel(window);

	nana::color value() const;
	void value(const nana::color& c);
protected:
	spinbox r_in{ *this };
	spinbox g_in{ *this };
	spinbox b_in{ *this };
	spinbox a_in{ *this };
	button preview{ *this };
	drawing preview_drawing{ preview };

	place plc{ *this };

private:
	void rgba_changed(const arg_spinbox& arg);
};
