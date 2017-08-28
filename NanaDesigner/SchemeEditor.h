#pragma once
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/button.hpp>
#include "color_picker_panel.h"
#include <iostream>
#include "Utils.h"

#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/date_chooser.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/msgbox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/menu.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/scroll.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/progress.hpp>
#include "nana_typeinfo.h"

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

using namespace nana;
using std::string;

struct SchemeEditorOption
{
	string name;
	widget* editor;

	SchemeEditorOption();
	SchemeEditorOption(const string& name, widget* wdg);
	~SchemeEditorOption();
};

class SchemeEditorOptionTag
{
public:
	inline SchemeEditorOptionTag(window widget_window) : widget_window(widget_window)
	{}

	inline void collocate_widget() const
	{
		API::refresh_window(widget_window);
	}

protected:
	window widget_window = nullptr;
};

class Color_SchemeEditorOption : public color_picker_panel, public SchemeEditorOptionTag
{
public:
	explicit Color_SchemeEditorOption(window editor_window, window widget_window, nana::color_proxy& color)
		: color_picker_panel(editor_window), SchemeEditorOptionTag(widget_window), color(color)
	{
		value(color);

		this->color_changed([this](const arg_color_picker& arg)
		{
			this->color = arg.color;
			API::refresh_window(this->widget_window);

			std::cout << "Refreshing window" << std::endl;
		});
	}

private:
	nana::color_proxy& color;
};

//wait this isn't a visual setting
class MouseParameter_SchemeEditorOption : public panel<true>, public SchemeEditorOptionTag
{
public:
	explicit MouseParameter_SchemeEditorOption(window editor_window, window widget_window, parameters::mouse_wheel& mouse_wheel)
		: panel<true>(editor_window), SchemeEditorOptionTag(widget_window), mouse_wheel(mouse_wheel)
	{
		lines.range(0, std::numeric_limits<int>::max(), 1);
		lines.value(std::to_string(mouse_wheel.lines));
		characters.range(0, std::numeric_limits<int>::max(), 1);
		characters.value(std::to_string(mouse_wheel.characters));

		lines.events().text_changed([this](const arg_spinbox& arg)
		{
			this->mouse_wheel.lines = arg.widget.to_int();

			API::refresh_window(this->widget_window);
		});
		characters.events().text_changed([&](const arg_spinbox& arg)
		{
			this->mouse_wheel.characters = arg.widget.to_int();

			API::refresh_window(this->widget_window);
		});

		plc.div("vert <lines> <chars>");
		plc["lines"] << lines;
		plc["chars"] << characters;
	}
	
protected:
	spinbox lines{*this};
	spinbox characters{*this};
	place plc{*this};

private:
	parameters::mouse_wheel& mouse_wheel;
};

template<typename T>
class Number_SchemeEditorOption : public spinbox, public SchemeEditorOptionTag
{
public:
	explicit Number_SchemeEditorOption(window editor_window, window widget_window, T& t, T min = std::numeric_limits<int>::min(), T max = std::numeric_limits<int>::max(), T step = 1)
		: spinbox(editor_window), SchemeEditorOptionTag(widget_window), t(t)
	{
		value(std::to_string(t));
		range(static_cast<int>(min), static_cast<int>(max), static_cast<int>(step));

		events().text_changed([this](const arg_spinbox& arg)
		{
			this->t = arg.widget.to_int();
		});
	}

private:
	T& t;
};

using SignedInt_SchemeEditorOption = Number_SchemeEditorOption<int>;
using UnsignedInt_SchemeEditorOption = Number_SchemeEditorOption<unsigned>;

template<typename SchemeType>
std::vector<SchemeEditorOption*> get_scheme_options(window editor_window, window widget_window, SchemeType& scheme_instance)
{
	//help its not specialized!
	//TODO: Make it throw when the build version is Debug
	if (std::is_default_constructible_v<SchemeType>) {
		SchemeType st;
		std::cerr << "No specialisation defined for SchemeType: " << typeid(st).name() << std::endl;
	}
	else
	{
		std::cerr << "No specialisation defined for the non-default constructable SchemeType" << std::endl;
	}

	return {};
}

/* widgets which are supported:
* - button(widget_geometrics)
* - checkbox(widget_geometrics)
* - combobox(widgets::skeletons::text_editor_scheme)
* - date_chooser(widget_geometrics)
* - group(widget_geometrics)
* - label(widget_geometrics)
* - listbox(drawerbase::listbox::scheme)
* - menubar(drawerbase::menubar::scheme)
* - panel(widget_geometrics)
* - picture(widget_geometrics)
* - progress(widget_geometrics)
* - slider(drawerbase::slider::scheme_impl)
* - spinbox(widgets::skeletons::text_editor_scheme)
* - tabbar(widget_geometrics)
* - textbox(widgets::skeletons::text_editor_scheme)
* - toolbar(widget_geometrics)
* - treebox(widget_geometrics)
*/

template<>
std::vector<SchemeEditorOption*> get_scheme_options<widget_geometrics>(window wd, window widget_window, widget_geometrics& scheme_editor);

template<>
std::vector<SchemeEditorOption*> get_scheme_options<combox::scheme_type>(window wd, window widget_window, combox::scheme_type& scheme_editor);

template<>
std::vector<SchemeEditorOption*> get_scheme_options<listbox::scheme_type>(window wd, window widget_window, listbox::scheme_type& scheme_editor);

template<>
std::vector<SchemeEditorOption*> get_scheme_options<menubar::scheme_type>(window wd, window widget_window, menubar::scheme_type& scheme_editor);

template<>
std::vector<SchemeEditorOption*> get_scheme_options<slider::scheme_type>(window wd, window widget_window, slider::scheme_type& scheme_editor);

//template<typename SchemeType>
//using get_editor_scheme_t = std::vector<SchemeEditorOption*>(*)(window wd, window widget_window, SchemeType& scheme_editor);
//extern std::map<nanatype::id_t, get_editor_scheme_t> schemeoption_function_map;

template<typename T>
class SchemeEditor : public form
{
public:
	using SchemeType = typename T::scheme_type;

	explicit SchemeEditor(window wd, T& widget)
		: form(wd), widget_scheme(widget.scheme())
	{
		caption("Scheme Editor");

		size({ 400, 200 });

		const char* div_code = "<vert labels weight=80><weight=10><vert editors>";
		div(div_code);

		std::vector<SchemeEditorOption*> raw_options = get_scheme_options<SchemeType>(*this, widget, widget_scheme);
		scheme_options.reserve(raw_options.size());
		labels.reserve(raw_options.size());

		for (SchemeEditorOption* raw_option : raw_options)
		{
			label* l = new label{ *this, raw_option->name};
			l->text_align(align::right, align_v::center);
			(*this)["labels"] << *l;
			(*this)["editors"] << *raw_option->editor;

			labels.emplace_back(l);
			scheme_options.emplace_back(raw_option);

			std::cout << raw_option->name  << std::endl;
		}

		collocate();
	}

protected:
	SchemeType& widget_scheme;
	std::vector<std::unique_ptr<SchemeEditorOption>> scheme_options;

private:
	std::vector<std::unique_ptr<label>> labels;
};



#undef option_color