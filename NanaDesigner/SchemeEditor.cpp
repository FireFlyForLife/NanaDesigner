#include "SchemeEditor.h"


SchemeEditorOption::SchemeEditorOption()
	: name(), editor(nullptr)
{
}

SchemeEditorOption::SchemeEditorOption(const string& name, widget* wdg)
	: name(name), editor(wdg)
{
}

//end class SchemeEditor 
SchemeEditorOption::~SchemeEditorOption()
{
	delete editor; editor = nullptr;
}

#define option_color(option_name) SchemeEditorOption* option_name = new SchemeEditorOption();\
	option_name->name = STR(option_name);\
	option_name->editor = new Color_SchemeEditorOption(wd, widget_window, scheme_editor.option_name);

#define option_unsigned(option_name) SchemeEditorOption* option_name = new SchemeEditorOption();\
	option_name->name = STR(option_name);\
	option_name->editor = new UnsignedInt_SchemeEditorOption(wd, widget_window, scheme_editor.option_name);

template <>
std::vector<SchemeEditorOption*> get_scheme_options<widget_geometrics>(window wd, window widget_window, widget_geometrics& scheme_editor)
{
	option_color(activated)
	option_color(background)
	option_color(foreground)

	return {activated, background, foreground};
}

template <>
std::vector<SchemeEditorOption*> get_scheme_options<widgets::skeletons::text_editor_scheme>(window wd, window widget_window, combox::scheme_type& scheme_editor)
{
	option_color(selection)
	option_color(selection_unfocused)
	option_color(selection_text)

	SchemeEditorOption* mwheel_option = new SchemeEditorOption();
	mwheel_option->name = "mouse_wheel - lines/characters";
	mwheel_option->editor = new MouseParameter_SchemeEditorOption(wd, widget_window, scheme_editor.mouse_wheel);

	std::vector<SchemeEditorOption*> options_1{selection, selection_unfocused, selection_text, mwheel_option};

	std::vector<SchemeEditorOption*> options = get_scheme_options<widget_geometrics>(wd, widget_window, scheme_editor);
	options.insert(options.end(), options_1.begin(), options_1.end());
	return options;
}

template <>
std::vector<SchemeEditorOption*> get_scheme_options<drawerbase::listbox::scheme>(window wd, window widget_window, listbox::scheme_type& scheme_editor)
{
	option_color(header_bgcolor)
	option_color(header_fgcolor)
	option_color(header_grabbed)
	option_color(header_floated)
	option_color(item_selected)
	option_color(item_highlighted)
	option_color(selection_box)

	option_unsigned(max_fit_content)
	option_unsigned(min_column_width)

	option_unsigned(suspension_width)
	option_unsigned(text_margin)
	option_unsigned(header_height)
	option_unsigned(item_height_ex)
	option_unsigned(header_splitter_area_before)
	option_unsigned(header_splitter_area_after)

	SchemeEditorOption* mwheel_option = new SchemeEditorOption();
	mwheel_option->name = "mouse_wheel - lines/characters";
	mwheel_option->editor = new MouseParameter_SchemeEditorOption(wd, widget_window, scheme_editor.mouse_wheel);

	std::vector<SchemeEditorOption*> options_1{header_bgcolor, header_fgcolor, header_grabbed, header_floated,
		item_selected, item_highlighted, selection_box, max_fit_content, min_column_width, suspension_width, text_margin,
		header_height, item_height_ex, header_splitter_area_before, header_splitter_area_after, mwheel_option};

	std::vector<SchemeEditorOption*> options = get_scheme_options<widget_geometrics>(wd, widget_window, scheme_editor);
	options.insert(options.end(), options_1.begin(), options_1.end());
	return options;
}

template <>
std::vector<SchemeEditorOption*> get_scheme_options<drawerbase::menubar::scheme>(window wd, window widget_window, menubar::scheme_type& scheme_editor)
{
	option_color(text_fgcolor)
	option_color(body_highlight)
	option_color(body_selected)
	option_color(border_selected)
	option_color(border_highlight)

	std::vector<SchemeEditorOption*> options_1{text_fgcolor, body_highlight, body_selected,
		border_highlight, border_selected};

	std::vector<SchemeEditorOption*> options = get_scheme_options<widget_geometrics>(wd, widget_window, scheme_editor);
	options.insert(options.end(), options_1.begin(), options_1.end());
	return options;
}

template <>
std::vector<SchemeEditorOption*> get_scheme_options<drawerbase::slider::scheme_impl>(window wd, window widget_window, slider::scheme_type& scheme_editor)
{
	option_color(color_adorn)
	option_color(color_bar)
	option_color(color_slider)
	option_color(color_slider_highlighted)
	option_color(color_vernier)
	option_color(color_vernier_text)

	option_unsigned(vernier_text_margin)

	std::vector<SchemeEditorOption*> options_1{color_adorn, color_bar, color_slider, color_slider_highlighted,
		color_vernier, color_vernier_text, vernier_text_margin};

	std::vector<SchemeEditorOption*> options = get_scheme_options<widget_geometrics>(wd, widget_window, scheme_editor);
	options.insert(options.end(), options_1.begin(), options_1.end());
	return options;
}

std::vector<SchemeEditorOption*> test1(window wd, window widget_window, slider::scheme_type& scheme_editor) { return{}; }

template<typename T>
std::vector<SchemeEditorOption*> test2(window wd, window widget_window, slider::scheme_type& scheme_editor) { return{}; }

template<>
std::vector<SchemeEditorOption*> test2<int>(window wd, window widget_window, slider::scheme_type& scheme_editor) { return{}; }


#define FUNC_ITEM(type) { GENERATE_NANATYPE_ID(type{}), &get_scheme_options<type::scheme_type> }

//std::conditional<std::is_same<widget_geometrics, spinbox::scheme_type>::value, widget_geometrics, spinbox::scheme_type>::type

//std::map<nanatype::id_t, get_editor_scheme_t> schemeoption_function_map{
//	{ typeid(button{}).name(), &get_scheme_options<drawerbase::menubar::scheme> }
//};


//template<>
//std::vector<std::unique_ptr<SchemeEditorOption>> get_scheme_options<::scheme_type>(window wd, ::scheme_type& scheme_editor)
//{
//	return{};
//}
