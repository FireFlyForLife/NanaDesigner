#pragma once
#include <string>
#include <map>
#include <memory>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>

using std::string;

namespace nana
{
	struct nanatype
	{
		std::string readable_name;
		std::string internal_name;
		std::string include;

		explicit nanatype(string readable_name = "", string internal_name = "", string include = "", string include_prefix = "nana/gui/widgets/")
		{
			this->readable_name = readable_name;
			this->internal_name = internal_name; 
			this->include = include_prefix + include;
		}

		
	};

//	template<typename T>
//	nanatype get_nanatype(T& widget)
//	{
//		static_assert(std::is_base_of<nana::widget, T>::value, "Type T function get_nanatype needs to be a subclass of nana::widget.");
//
//		string name = typeid(widget).name();
//		string rawname = typeid(widget).raw_name();
//		return nanatype(name, rawname, rawname + "=UNKNOWN CLASS");
//	}

//	template<>
//	nanatype get_nanatype(button& widget)
//	{
//		return nanatype("Button", "button", "button.hpp");
//	}

	//TODO: Create class around this for ease of access
	extern std::map<string, std::shared_ptr<nanatype>> types;

	nanatype& get_nanatype(widget& widget);

	//static std::vector<nanatype<>> types{ nanatype<button>("Button", "button", "button.hpp") };

	/*std::array<nanatype, 3> types{ nanatype<button>() };
	std::vector<nanatype> types{ nanatype<button>("Button", "button", "button.hpp") };

	std::vector<std::shared_ptr<nanatype<widget>>> getall();*/

//	std::vector<std::shared_ptr<nanatype<widget>>> types
//	{
//		std::shared_ptr<nanatype<button>>(new nanatype<button>("Button", "button", "button.hpp"))
//
//	};
}
