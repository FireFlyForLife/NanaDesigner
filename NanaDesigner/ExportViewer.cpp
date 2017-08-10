#include "ExportViewer.h"
#include "Utils.h"
#include <nana/system/dataexch.hpp>
#include <map>
#include "vector_utils.h"
#include "DesignerForm.h"
#include "format.h"
#include <filesystem>

namespace fs = std::experimental::filesystem;

static nana::rectangle get_absolute_rectangle(int width, int height, window owner)
{
	nana::rectangle ret = API::make_center(width, height);
	point screencoord{ 0, 0 };
	API::calc_window_point(owner, screencoord);
	ret.position(ret.position() + screencoord);
	return ret;
}

ExportViewer::ExportViewer(window window_handle) : form(window_handle, get_absolute_rectangle(300, 700, window_handle))
{
	caption("Exported Form");

	const char* divText = MULTILINE(
		vert
		<max=30 <fit lblFormName><margin = [3, 5] txtFormName>>
		|
		<max=30
			<fit lblHeader>
			<margin = [3, 5] btnCopyHeader>
		>
		<txtHeader>
		|
		<max=30
			<fit lblSource>
			<margin = [3, 5] btnCopySource>
		>
		<txtSource>
	);
	div(divText);

	headerCopy.events().click([&](arg_click click)
	{
		clipboard.set(headerText.caption(), native_handle());
	});

	sourceCopy.events().click([&](arg_click)
	{
		clipboard.set(sourceText.caption(), native_handle());
	});

	formName.multi_lines(false).text_align(align::left);
	formName.events().text_changed([&](const arg_textbox& arg)
	{
		form_name = formName.caption();
		RefreshText();
	});
	formName.set_accept(&ExportViewer::isCorrectChar);

	(*this)["lblFormName"] << formNameLabel;
	(*this)["txtFormName"] << formName;
	(*this)["lblHeader"] << headerLabel;
	(*this)["btnCopyHeader"] << headerCopy;
	(*this)["txtHeader"] << headerText;
	(*this)["lblSource"] << sourceLabel;
	(*this)["btnCopySource"] << sourceCopy;
	(*this)["txtSource"] << sourceText;

	collocate();
}

const char* HEADER_TEMPLATE = "#pragma once\n\n" "#include <nana/gui/widgets/form.hpp>\n" "{0}" "\n\n" MULTILINE(
using namespace nana; \n\
\n\
class {2} : public form {{
\n\
public:\n\
    {2}(); \n\
	\n\
protected:\n\
    {1}\
}};\n
);

const char* SOURCE_TEMPLATE = "#include \"{3}.h\"\n\n{1}" MULTILINE({3}::{3}()\n\
{{\n\
    caption("my generated form!");\n\
    \n\
    {2}\
    \n\
	{0}\
    collocate();\n\
}}
);

void ExportViewer::GenerateCode(PreviewPanel& preview, project_info* info)
{
	formName.caption(info ? info->name : "myform");
	json contents = GenerateCodeContents(preview);
	
	const string div = contents["div"];

	string widget_declaration;
	string widget_assign;
	for (auto widget : contents["widgets"])
	{
		string type = widget["type"];
		widget_declaration += ("    " + type + " ");
		string widgetname = widget["name"];
		widget_declaration += widgetname;
		string caption = widget["caption"];
		widget_declaration += ("{*this, \"" + caption + "\" };\n");

		string tag = widget["tag"];
		widget_assign += ("    (*this)[\"" + tag + "\"] << " + widgetname + ";\n");
	}

	//header generation
	for (string incl : contents["includes"])
		includes.append("#include <" + incl + ">\n");

	form_name = formName.caption();

	header_widgetlist = widget_declaration;

	//source generation
	bool use_multiline = div.find('\n') != std::string::npos;
	multiline_declaration = "";
	if (use_multiline)
		multiline_declaration = "#define MULTILINE(...) #__VA_ARGS__" "\n\n";

	if (use_multiline)
		div_code = "div(MULTILINE(\n" + div + "\n));\n";
	else
		div_code = "div(\"" + div + "\");\n";

	widget_assignment = widget_assign;

	RefreshText();

}

void ExportViewer::RefreshText()
{
	string header_code = fmt::format(HEADER_TEMPLATE, includes, header_widgetlist, form_name);
	string source_code = fmt::format(SOURCE_TEMPLATE, widget_assignment, multiline_declaration, div_code, form_name);

	headerText.caption(header_code);
	sourceText.caption(source_code);
}

json ExportViewer::GenerateCodeContents(PreviewPanel& preview)
{
	json contents = json::object({
		{"div", ""},
		{"includes", json::array()},
		{"widgets", json::array()}
	});
	
	string div = preview.getDiv();
	contents["div"] = div;

	std::map<string, int> tags;
	std::vector<string> incls;
	for (int i = 0; i < preview.widgetAmount(); ++i)
	{
		auto& pair = preview.getWidget(i);
		string tag = pair.first;

		if (tags.count(tag) > 0)
			tags[tag]++;
		else
			tags[tag] = 0;

		int tagcount = tags[tag];
		string widgetname = !tagcount ? pair.first : pair.first + std::to_string(tagcount);

		nanatype type;

		try
		{
			type = nana::get_nanatype(*pair.second);
		}
		catch (std::exception e)
		{
			std::cout << "Could not find " << typeid(*pair.second).name() << "in type set" << std::endl;
			type = nanatype(typeid(*pair.second).name(), typeid(*pair.second).raw_name(), (string(typeid(*pair.second).name()) + "UNKNOWN TYPE").c_str());
		}

		if(vector_indexof(incls, type.include) == -1)
		{
			incls.push_back(type.include);
		}

		contents["widgets"].push_back({
			{"type", type.internal_name},
			{"tag", tag},
			{"name", widgetname},
			{"caption", pair.second->caption()}
		});
	}

	contents["includes"] = incls;	

	return contents;
}

bool ExportViewer::isCorrectChar(wchar_t c)
{
	bool isText = ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
	bool isNum = ('0' <= c && c <= '9');
	bool isUnderscore = (c == '_');
	bool backspaceOrDelete = (c == 8 || c == 46);

	return isText || isNum || isUnderscore || backspaceOrDelete;
}

bool ExportViewer::isCorrectName(const string& filename)
{
	for (const char c : filename)
	{
		if (!ExportViewer::isCorrectChar(c))
			return false;
	}

	return true;
}
