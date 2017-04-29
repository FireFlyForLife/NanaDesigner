#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/panel.hpp>

#include <nana/gui/msgbox.hpp>
#include <nana/gui/filebox.hpp>
#include <thread>

#include "WidgetAdder.h"
#include "PreviewPanel.h"

#include <string>
#include <iostream>

using namespace nana;
using namespace std;

int main()
{
	using namespace nana;
	using namespace std;

	//Define a form.
	form fm;
	fm.caption("Nana UI Editor");

	//Define a button and answer the click event.
	button btn{ fm, "Quit" };
	btn.events().click([&fm] {
		fm.close();
	});

	//Layout management
	fm.div("vert <menubar weight=25>< <toolkit max=60> <vert <dock <preview>> <vert <code><<widgets>< <margin=[5,5] <apply_div>> >> > > >");

	place& root = fm.get_place();
	place& dockable_root = root.dock<PreviewPanel>("preview", "f");
	widget* dock = root.dock_create("f");

	PreviewPanel& pre = *static_cast<PreviewPanel*>(dock);
	
	pre.applyRawDiv("<><btn>");
	pre.addWidget<button>("btn", "Buttaaan!");
	pre.refresh();
	
	menubar menuBar{ fm };
	menu& file_menu = menuBar.push_back("File");
	file_menu.append("New", [&dockable_root](nana::menu::item_proxy& ip) {
		dockable_root.dock_create("f");
	});
	
	file_menu.append("Save");
	file_menu.append("Open");

	menuBar.push_back("Close");
	button btnCreate{ fm, "Create" };

	textbox divcode{ fm, ""};
	divcode.multi_lines(true).editable(true);
	
	textbox widgetlist{ fm, "" };
	widgetlist.multi_lines(true).editable(false);

	button apply_div{ fm, "Apply" };
	apply_div.events().click([&](const nana::arg_click&) {
		string txt = divcode.caption();
		
		pre.applyRawDiv(txt);
		});

	fm["menubar"] << menuBar;
	fm["toolkit"] << btnCreate;
	fm["code"] << divcode;
	fm["widgets"] << widgetlist;
	fm["apply_div"] << apply_div;

	fm.collocate();
	fm.show();

	//Start to event loop process, it blocks until the form is closed.
	exec();
}