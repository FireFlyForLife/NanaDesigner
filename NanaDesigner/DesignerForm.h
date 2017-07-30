#pragma once

#include "nana/gui.hpp"
#include <nana/gui/widgets/menu.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/group.hpp>

#include "PreviewPanel.h"
#include <nana/gui/widgets/textbox.hpp>
#include <stack>
#include "ExportViewer.h"
#include "nana_typeinfo.h"

using namespace nana;

class DesignerForm : public form
{
public:
	DesignerForm();
	virtual ~DesignerForm();

	PreviewPanel* GetPreviewPanel() const;

	void Refresh();
	void RefreshWidgetList();
	void RefreshDivText();

protected:
	template<typename T>
	void AddCreateWidgetButton(const string& widget_name)
	{
		std::unique_ptr<button> btn_ptr = std::make_unique<button>(creating_group, widget_name);
		btn_ptr->events().click([this]{
			inputbox::text tagText{ "Placement tag" };
			inputbox::text captionText{ "Widget caption" };

			//TODO: Make a thread about improving this class, right now it looks awful
			inputbox ib{ *this, "Enter the widget information                                             ", "Widget Attributes" };
			ib.verify([&tagText](window wd)
			{
				return !tagText.value().empty();
			});

			if(ib.show(tagText, captionText))
			{
				string tag = tagText.value();
				string caption = captionText.value();

				preview->addWidget<T>(tag, caption);
				preview->refresh();
				RefreshWidgetList();
			}
		});
		
		creating_group["creating"] << *btn_ptr;
		create_buttons.push_back(std::move(btn_ptr));
	}

protected:
	menubar menuBar{ *this };
	menu& file_menu = menuBar.push_back("File");
	menu& preview_menu = menuBar.push_back("Preview");
	menu& schema_menu = menuBar.push_back("Schema");
	
	PreviewPanel* preview = nullptr;

	textbox divcode{ *this };
	listbox widgetlist{ *this };
	button apply_div{ *this, "Apply" };

	group creating_group{ *this };

	group editing_group{ *this };
	button* btnEdit = editing_group.create_child<button>("editing", "Edit");
	button* btnRemove = editing_group.create_child<button>("editing", "Remove");

	std::vector<std::unique_ptr<button>> create_buttons;

private:
	//std::vector<std::unique_ptr<ExportViewer>> export_viewers;
	std::vector < std::unique_ptr<form>> children;
};
