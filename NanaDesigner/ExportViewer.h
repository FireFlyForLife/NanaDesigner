#pragma once
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/system/dataexch.hpp>
#include "PreviewPanel.h"
#include "json.hpp"
#include "nana_typeinfo.h"

struct project_info;
using namespace nana;
using json = nlohmann::json;

class ExportViewer : public form
{
public:
	ExportViewer(window window_handle);

	void RefreshText();
	void GenerateCode(PreviewPanel& preview, project_info* info = nullptr);
	static json GenerateCodeContents(PreviewPanel& preview);

	//TODO: Add this to a utility file
	static bool isCorrectChar(wchar_t c);
	static bool isCorrectName(const string& filename);

protected:
	textbox formName{ *this };
	label formNameLabel{ *this, "Form name:" };

	label headerLabel{ *this, "Header file:" };
	button headerCopy{ *this, "Copy to Clipboard" };
	textbox headerText{ *this };

	label sourceLabel{ *this, "Source file:" };
	button sourceCopy{ *this, "Copy to Clipboard" };
	textbox sourceText{ *this };

	string form_name;
	string header_widgetlist;
	string includes;

	string widget_assignment;
	string div_code;
	string multiline_declaration;


	system::dataexch clipboard;
};
