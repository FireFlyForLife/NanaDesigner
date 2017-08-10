#pragma once
#include <string>
#include <map>
#include <memory>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>

#define GENERATE_NANATYPE_ID(widget) typeid(widget).name()

using std::string;

//TODO: Should this really be in the nana namespace?
namespace nana
{
	struct nanatype
	{
		using id_t = string;

		std::string readable_name;
		std::string internal_name;
		std::string include;

		explicit nanatype(string readable_name = "", string internal_name = "", string include = "", string include_prefix = "nana/gui/widgets/")
		{
			this->readable_name = readable_name;
			this->internal_name = internal_name; 
			this->include = include_prefix + include;
		}

		virtual ~nanatype(){}

		virtual widget* instantiate(window handle = nullptr, const string& caption = "");
	};

	template<typename T>
	struct specific_nanatype : public nanatype
	{
		explicit specific_nanatype(string readable_name = "", string internal_name = "", string include = "", string include_prefix = "nana/gui/widgets/")
			: nanatype(readable_name, internal_name, include, include_prefix)
		{
			static_assert(std::is_base_of<nana::widget, T>::value, "Type T nana::specific_nanatype needs to be a subclass of nana::widget.");
		}

		widget* instantiate(window handle = nullptr, const string& caption = "") override
		{
			T* t;
			if (!handle)
				t = new T{};
			else
				t = new T{ handle, caption };

			return t;
		}
	};

	//TODO: Create class around this for ease of access
	extern std::map<nanatype::id_t, std::shared_ptr<nanatype>> types;

	nanatype& get_nanatype(widget& widget);
	nanatype& get_nanatype_by_readablename(const string& readable_name);
	nanatype& get_nanatype_by_internalname(const string& internal_name);

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
