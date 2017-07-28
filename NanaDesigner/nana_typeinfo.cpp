#include "nana_typeinfo.h"

#define typeitem(T, readable, core, incl) { typeid(T{}).name(), std::make_shared<nanatype>(readable, core, incl) } //TODO: Fix memory leak

using namespace nana;

std::map<string, std::shared_ptr<nana::nanatype>> nana::types
{
	typeitem(button, "Button", "button", "button.hpp"),
	typeitem(label, "Label", "label", "label.hpp"),
	typeitem(textbox, "Textbox", "textbox", "textbox.hpp"),
	typeitem(checkbox, "Checkbox", "checkbox", "checkbox.hpp")
};

nanatype& nana::get_nanatype(widget& widget)
{
	string name = typeid(widget).name();
	if (types.count(name) > 0)
	{
		return *types.at(name);
	}

	string msg{ "No widget was found with name" + name };
	throw new std::exception(msg.c_str());
}
