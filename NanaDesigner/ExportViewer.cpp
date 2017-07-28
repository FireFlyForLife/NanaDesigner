#include "ExportViewer.h"
#include "Utils.h"
#include <nana/system/dataexch.hpp>
#include <map>
#include "vector_utils.h"

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

	(*this)["lblHeader"] << headerLabel;
	(*this)["btnCopyHeader"] << headerCopy;
	(*this)["txtHeader"] << headerText;
	(*this)["lblSource"] << sourceLabel;
	(*this)["btnCopySource"] << sourceCopy;
	(*this)["txtSource"] << sourceText;

	collocate();
}

const char* headerIncludeStart = "#include <nana/gui/widgets/form.hpp>\n";

const char* headerStartTemplate = "\n\n" MULTILINE(
using namespace nana;\n\
\n\
class myform : public form{\n\
public:\n\
    myform();\n\
\n\
protected:\n\

);

const char* headerEndTemplate = MULTILINE(\
};
);

const char* sourceStartTemplate = "#include \"myform.h\"\n" "\n\n" MULTILINE(
myform::myform()\n\
{\n\
) "    caption(\"my generated form!\");\n\n";

const char* sourceEndTemplate = MULTILINE(\
	collocate();\n\
}
);

void ExportViewer::GenerateCode(PreviewPanel& preview)
{
	json contents = GenerateCodeContents(preview);

	const string div = preview.getDiv();

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
//		int tagcount = tags[pair.first];
//		string widgetname = !tagcount ? pair.first : pair.first + std::to_string(tagcount);
//		widget_declaration += widgetname;
//
//		widget_declaration += "{ *this, \"";
//		widget_declaration += pair.second->caption();
//		widget_declaration += "\" }; \n";
//
//		widget_assign += "    ";
//		widget_assign += "(*this)[\"";
//		widget_assign += pair.first;
//		widget_assign += "\"] << ";
//		widget_assign += widgetname;
//		widget_assign += ";\n";
	}

	string header(headerIncludeStart);
	for (string incl : contents["includes"])
		header.append("#include <" + incl + ">\n");
	header.append(headerStartTemplate);
	header.append(widget_declaration);
	header.append(headerEndTemplate);
	headerText.caption(header);

	string src(sourceStartTemplate);
	src.append("    div(\"");
	src.append(div);
	src.append("\");\n\n");
	src.append(widget_assign);
	src.append(sourceEndTemplate);
	sourceText.caption(src);
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

//		button* btn = dynamic_cast<button*>(pair.second.get());
//		nanatype type = nana::get_nanatype(*btn);

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
