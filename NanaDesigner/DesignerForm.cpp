#include "DesignerForm.h"
#include <iostream>
#include <nana/gui/widgets/label.hpp>

#define MULTILINE(...) #__VA_ARGS__

DesignerForm::DesignerForm() : form(API::make_center(800, 600))
{
	caption("Nana UI Editor");

	const char* main_div_text = MULTILINE(
	vert <menubar weight = 25> <
		<max = 60 vert
			<creating>
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
	div(main_div_text);

	get_place().dock<PreviewPanel>("preview", "f");
	widget* dock = get_place().dock_create("f");
	preview = static_cast<PreviewPanel*>(dock);

	file_menu.append("New", [this](nana::menu::item_proxy& ip) {
		this->get_place().dock_create("f");
	});

	file_menu.append("Save");
	file_menu.append("Open");

	const char* creating_group_div = MULTILINE(
		<vert creating margin=[0, 3] gap=5>
	);
	creating_group.caption("Create");
	creating_group.div(creating_group_div);

	const char* editing_group_div = MULTILINE(
		<vert editing>
	);
	editing_group.caption("Editing");
	editing_group.div(editing_group_div);

	divcode.multi_lines(true).editable(true);
	divcode.caption("form.div(\"" + preview->getDiv() + "\");");

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

	apply_div.events().click([&](const nana::arg_click&) {
		string txt = divcode.caption();
		
		//TODO: Is there a method to check the div first?
		string oldDiv = preview->getDiv();
		try {
			preview->applyRawDiv(txt);
		}
		catch (std::exception ex) {
			preview->applyDiv(oldDiv);
			msgbox mb{ *this, "Error applying div text", msgbox::ok };
			mb.icon(msgbox::icon_error);
			mb << "Error applying div text: '" << ex.what() << "'";
			mb.show();
			std::cout << "Error applying div text: " << ex.what() << std::endl;
		}
	});

	(*this)["menubar"] << menuBar;
	(*this)["creating"] << creating_group;
	(*this)["editing"] << editing_group;
	(*this)["code"] << divcode;
	(*this)["widgets"] << widgetlist;
	(*this)["apply_div"] << apply_div;

	AddCreateWidgetButton<button>("Button");
	AddCreateWidgetButton<textbox>("Textbox");
	AddCreateWidgetButton<checkbox>("checkbox");
	AddCreateWidgetButton<label>("label");

	collocate();
}

PreviewPanel* DesignerForm::GetPreviewPanel() const
{
	return preview;
}

void DesignerForm::Refresh()
{
	preview->refresh();
	RefreshWidgetList();
	RefreshDivText();
}

void DesignerForm::RefreshWidgetList()
{
	//TODO: Make this actually refresh and not just scrap and push again

	auto cat = widgetlist.at(0);
	if(cat.size() > 0) //TODO: Shouldn't this check be in the clear method itself?
		widgetlist.clear(0);

	int widgetAmount = preview->widgetAmount();
	for (int i = 0; i < widgetAmount; ++i)
	{
		auto pair = preview->getWidget(i);
		cat.append({ pair.first, typeid(*pair.second).name(), pair.second->caption() }); //TODO: Create a 'dictionairy' of the widget types to readable strings
	}
}

void DesignerForm::RefreshDivText()
{
	divcode.caption("form.div(\"" + preview->getDiv() + "\");");
}
