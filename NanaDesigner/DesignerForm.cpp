#include "DesignerForm.h"
#include <iostream>
#include <nana/gui/widgets/label.hpp>
#include "Utils.h"
#include "ExportViewer.h"
#include "ResolutionForm.h"
#include <fstream>
#include <nana/gui/filebox.hpp>
#include <filesystem>
#include <nana/gui/widgets/spinbox.hpp>
#include "color_picker_panel.h"
#include "SchemeEditor.h"

namespace fs = std::experimental::filesystem;

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
	preview = dynamic_cast<PreviewPanel*>(dock);

	file_menu.append("New", [this](nana::menu::item_proxy& ip) {
		this->get_place().dock_create("f");
	});

	file_menu.append("Save", [&](menu::item_proxy& ip)
	{
		filebox fb{*this, false};
		fb.title("GUI Project save file");
		fb.add_filter("JSON", "*.json;*.JSON");
		if (fb.show()) {
			string filename = (fs::path{ fb.file() }.filename().stem().string());
			std::cout << filename << std::endl;
			if (ExportViewer::isCorrectName(filename)) {
				if (!project_info_) project_info_ = new project_info();
				project_info_->name = filename;
				std::ofstream ofs(fb.file(), std::ios::binary);
				json j = json::object();
				j["name"] = project_info_->name;
				j["contents"] = ExportViewer::GenerateCodeContents(*preview);
				ofs << j;
				RefreshProjectInfo();
			}
			else
			{
				msgbox mb{ *this, "Wrong filename", msgbox::button_t::ok };
				mb.icon(msgbox::icon_error);
				mb << L"You can only enter the characters: [a-z] [A-Z] [0-9] and '_'";
				mb.show();
			}
		}
	});
	file_menu.append("Open", [&](menu::item_proxy& ip)
	{//TODO: Check for unsaved changes
		filebox fb{ *this, true };
		fb.title("GUI Project save file");
		fb.add_filter("JSON", "*.json;*.JSON");
		if (fb.show()) {
			std::ifstream ifs(fb.file(), std::ios::binary);
			json j;
			j << ifs;
			if (!project_info_) project_info_ = new project_info();
			string proj_name = j["name"];
			project_info_->name = proj_name;
			json& contents = j["contents"];
			string div = contents["div"];
			preview->applyDiv(div);
			for (int i = preview->widgetAmount() - 1; i >= 0; --i)
				preview->removeWidget(i);
			for (json object : contents["widgets"])
			{
				nanatype* type = &nana::get_nanatype_by_internalname(object["type"]);
				string caption = object["caption"];
				widget* widget = type->instantiate(*preview, caption);
				if(widget)
				{
					string tag = object["tag"];

					PreviewPanel::widget_ptr widget_ptr{ widget };
					PreviewPanel::widget_pair widget_pair{ tag, widget_ptr };

					preview->addWidget(widget_pair);
				}
				else
				{
					std::cout << "could not create widget: " << object["type"] << std::endl;
				}
			}

			Refresh();
		}
	});
	file_menu.append_splitter();
	file_menu.append("Export", [&](nana::menu::item_proxy& ip)
	{
		std::unique_ptr<ExportViewer> export_viewer(new ExportViewer(handle()));
		json j = export_viewer->GenerateCodeContents(*preview);//TODO: Handle no preview window available
		std::cout << j << std::endl;
		export_viewer->GenerateCode(*preview, project_info_);
		export_viewer->show();
		children.push_back(std::move(export_viewer));

	});
	preview_menu.append("Resolution", [this](menu::item_proxy& ip)
	{
		if(preview)
		{
			std::unique_ptr<ResolutionForm> resolution_form(new ResolutionForm(handle(), *preview));
			resolution_form->show();
			children.push_back(std::move(resolution_form));
		}
	});

	schema_menu.append("Scheme editor", [this](menu::item_proxy& ip)
	{
		auto indices = widgetlist.selected();
		for (auto selected : indices)
		{
			auto item = widgetlist.at(selected);
			PreviewPanel::widget_pair& pair_ptr = item.value<PreviewPanel::widget_pair>();
			string tag = pair_ptr.first;
			PreviewPanel::widget_ptr widget_ptr = pair_ptr.second;

			nanatype& type = nana::get_nanatype(*widget_ptr);
			form* scheme_editor = type.get_scheme_editor(*this, widget_ptr.get());
			
			//SchemeEditor<button>* scheme_editor = new SchemeEditor<button>(*this, apply_div);
			scheme_editor->show();
			children.emplace_back(scheme_editor);
		}

		if(indices.size() <= 0)
		{
			msgbox err{ *this, "Please select a widget", msgbox::button_t::ok };
			err << "No widget selected in the widgetlist";
			err.show();
		}
	});

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
	
	btnEdit->events().click([&](const arg_click& click)
	{
		auto indices = widgetlist.selected();
		for (auto selected : indices)
		{
			auto item = widgetlist.at(selected);
			PreviewPanel::widget_pair& pair_ptr = item.value<PreviewPanel::widget_pair>();
			string oldtag = pair_ptr.first;
			inputbox::text tagText{ "Placement tag", oldtag };
			inputbox::text captionText{ "Widget caption", pair_ptr.second->caption() };

			inputbox ib{ *this, "Enter the widget information                                             ", "Widget Attributes" };
			ib.verify([&tagText](window wd)
			{
				return !tagText.value().empty();
			});

			if (ib.show(tagText, captionText))
			{
				string tag = tagText.value();
				string caption = captionText.value();

				pair_ptr.second->caption(caption);
				if (tag != oldtag)
				{
					preview->removeWidget(pair_ptr);
					pair_ptr.first = tag;
					preview->addWidget(pair_ptr);
				}

				preview->refresh();
				RefreshWidgetList();
			}
		}
	});

	btnRemove->events().click([&](const arg_click& click)
	{
		auto indices = widgetlist.selected();
		for( auto selected : indices)
		{
			auto item = widgetlist.at(selected);
			PreviewPanel::widget_pair& pair_ptr = item.value<PreviewPanel::widget_pair>();
			preview->removeWidget(pair_ptr);
		}

		RefreshWidgetList();
		preview->refresh();
	});

	divcode.multi_lines(true).editable(true);
	color fg(colors::blue_violet);
	color bg(colors::white);
	divcode.set_highlight("default", fg, bg);
	divcode.set_keywords("default", true, true,
	{ "|","%","arrange","collapse","gap","grid","margin","min","max","repeated","variable","vertical","vert","weight" });
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
	widgetlist.scheme();

	RefreshWidgetList();

	apply_div.events().click([&](const nana::arg_click&) {
		string txt = divcode.caption();
		
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

	preview->show();
}

DesignerForm::~DesignerForm()
{
}

PreviewPanel* DesignerForm::GetPreviewPanel() const
{
	return preview;
}

void DesignerForm::Refresh()
{
	preview->refresh();
	RefreshProjectInfo();
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
		cat.append(pair, true);
	}
}

void DesignerForm::RefreshDivText()
{
	divcode.caption("form.div(\"" + preview->getDiv() + "\");");
}

void DesignerForm::RefreshProjectInfo()
{
	if(project_info_)
	{
		caption(project_info_->name + " - " + title);
	}
	else
	{
		caption("Untitled" " - " + title);
	}
}

listbox::oresolver& operator<<(listbox::oresolver& ores, const PreviewPanel::widget_pair& pair)
{
	return ores << pair.first << nana::get_nanatype(*pair.second).readable_name << pair.second->caption();
}
