#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/group.hpp>

#include <nana/gui/msgbox.hpp>
#include <nana/gui/filebox.hpp>
#include <thread>

#include "WidgetAdder.h"
#include "PreviewPanel.h"

#include <string>
#include <iostream>

#define MULTILINE(...) #__VA_ARGS__

using namespace nana;
using namespace std;



int main()
{
	using namespace nana;
	using namespace std;

	form fm{ API::make_center(800, 600) };

	menubar menuBar{ fm };
	menu& file_menu = menuBar.push_back("File");
	menu& close_menu = menuBar.push_back("Close");

	PreviewPanel* preview;

	button btnCreate{ fm, "Create" };

	group editing_group{ fm };
	button* btnRemove = editing_group.create_child<button>("remove", "Remove");

	//Edit the form.
	fm.caption("Nana UI Editor");

	//Define a button and answer the click event.
	button btn{ fm, "Quit" };
	btn.events().click([&] {
		fm.close();
	});

	//Layout management
	/*const char* main_divtext = 
	"vert <menubar weight=25> < \
		<toolkit max=60> \
		<vert <dock <preview>> \
			<vert \
				<code>\
				<\
					<widgets>\
					<margin=[5,5] apply_div> \
				> \
			> \
		> \
	>";*/

	const char* main_div_text = MULTILINE(
	vert <menubar weight = 25> <
		<max = 60 vert
			<toolkit> 
			<editing>
		>
		<vert <dock <preview>> 
			<vert 
				<code>
				<
					<widgets>
					<margin = [5, 5] apply_div> 
				> 
			>  
		> 
	>
	);
	
	fm.div(main_div_text);

	place& root = fm.get_place();
	root.dock<PreviewPanel>("preview", "f");
	widget* dock = root.dock_create("f");

	preview = static_cast<PreviewPanel*>(dock);
	
	preview->applyRawDiv("<><btn>");
	preview->addWidget<button>("btn", "Buttaaan!");
	preview->refresh();
	
	file_menu.append("New", [&root](nana::menu::item_proxy& ip) {
		root.dock_create("f");
	});
	
	file_menu.append("Save");
	file_menu.append("Open");

	editing_group.caption("Editing");
	editing_group.div("vert <remove>");

	//divcode.tip_string("place.div(\"<btn>\");");
	

	//editing_group["remove"] << btnRemove;

	textbox divcode{ fm};
	divcode.multi_lines(true).editable(true);
	divcode.caption("form.div(\"" + preview->getDiv() + "\");");

	listbox widgetlist{ fm };
	widgetlist.append_header("Field");
	widgetlist.append_header("Type");
	widgetlist.append_header("Caption");

	widgetlist.events().category_dbl_click([&](const arg_listbox_category& arg)
	{
		std::cout << "Clicked!" << std::endl;
	});

	widgetlist.events().selected([&](const arg_listbox& arg)
	{
		std::cout << "Clicked item! " << arg.item.selected() << std::endl;
	});

	auto cat = widgetlist.at(0);
	int widgetAmount = preview->widgetAmount();
	for (int i = 0; i < widgetAmount; ++i)
	{
		auto pair = preview->getWidget(i);
		cat.append({ pair.first, typeid(*pair.second).name(), pair.second->caption() }); //TODO: Create a 'dictionairy' of the widget types to readable strings
	}

	button apply_div{ fm, "Apply" };
	apply_div.events().click([&](const nana::arg_click&) {
		string txt = divcode.caption();
		
		preview->applyRawDiv(txt);
		});

	fm["menubar"] << menuBar;
	fm["toolkit"] << btnCreate;
	fm["editing"] << editing_group;
	fm["code"] << divcode;
	fm["widgets"] << widgetlist;
	fm["apply_div"] << apply_div;

	fm.collocate();
	fm.show();

	exec();
}