#include "color_picker_panel.h"
#include <iostream>

color_picker_panel::arg_color_picker::arg_color_picker(color_picker_panel& cpc, nana::color color)
	: color_picker(cpc), color(color)
{
}

color_picker_panel::color_picker_panel(window wd) : panel<true>(wd)
{
	plc.div("<arrange=[variable, variable, variable, variable, 40] <r margin=3><g margin=3><b margin=3><a margin=3><preview margin=3>>");
	
	r_in.range(0, 255, 1); r_in.value(std::to_string(200));
	g_in.range(0, 255, 1);
	b_in.range(0, 255, 1);
	a_in.range(0.0, 1.0, 0.1);

	preview_drawing.draw([this](paint::graphics& g)
	{
		g.rectangle(true, this->value());
		g.rectangle(false, colors::black);
	});
	preview_drawing.update();
	
	std::function<void(const arg_spinbox& arg)> func = std::bind(&color_picker_panel::rgba_changed, static_cast<color_picker_panel*>(this), std::placeholders::_1); //[&](const arg_spinbox& arg) {}; 
	r_in.events().text_changed(func);
	g_in.events().text_changed(func);
	b_in.events().text_changed(func);
	a_in.events().text_changed(func);

	plc["r"] << r_in;
	plc["g"] << g_in;
	plc["b"] << b_in;
	plc["a"] << a_in;
	plc["preview"] << preview;
}

nana::color color_picker_panel::value() const
{
	unsigned r = r_in.to_int();
	unsigned g = g_in.to_int();
	unsigned b = b_in.to_int();
	double a = a_in.to_double();

	return { r, g, b, a };
}

void color_picker_panel::value(const nana::color& c)
{
	r_in.value(std::to_string(c.r()));
	g_in.value(std::to_string(c.g()));
	b_in.value(std::to_string(c.b()));
	a_in.value(std::to_string(c.a()));
}

void color_picker_panel::rgba_changed(const arg_spinbox& arg)
{
	arg_color_picker cp{ *this, value() };
	color_changed.emit(cp, this->handle());
	preview_drawing.update();
}
