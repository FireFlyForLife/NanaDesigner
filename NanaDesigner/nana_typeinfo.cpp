#include "nana_typeinfo.h"

using namespace nana;

#define typeitem(T, readable, core, incl) { GENERATE_NANATYPE_ID(T{}), std::make_shared<specific_nanatype<T>>(readable, core, incl) }


std::map<nanatype::id_t, std::shared_ptr<nana::nanatype>> nana::types
{
	typeitem(button, "Button", "button", "button.hpp"),
	typeitem(label, "Label", "label", "label.hpp"),
	typeitem(textbox, "Textbox", "textbox", "textbox.hpp"),
	typeitem(checkbox, "Checkbox", "checkbox", "checkbox.hpp")
};

widget* nanatype::instantiate(window handle, const string& caption)
{
	return nullptr;
}

nanatype& nana::get_nanatype(widget& widget)
{
	nanatype::id_t name = GENERATE_NANATYPE_ID(widget);
	if (types.count(name) > 0)
	{
		return *types.at(name);
	}

	string msg{ "No widget was found with id" + name };
	throw std::exception(msg.c_str());
}

nanatype& nana::get_nanatype_by_readablename(const string& readable_name)
{
	for (auto pair : types)
	{
		auto type = pair.second;
		if (type->readable_name == readable_name)
			return *type;
	}

	string msg{ "No widget was found with readable_name: " + readable_name};
	throw std::exception(msg.c_str());
}

nanatype& nana::get_nanatype_by_internalname(const string& internal_name)
{
	for (auto pair : types)
	{
		auto type = pair.second;
		if (type->internal_name == internal_name)
			return *type;
	}

	string msg{ "No widget was found with internal_name: " + internal_name };
	throw std::exception(msg.c_str());
}
