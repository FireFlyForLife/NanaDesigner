#pragma once
#include <nana/gui.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/place.hpp>

#include <vector>
#include <memory>

using namespace nana;
using std::string;

class PreviewPanel : public panel<true>
{
protected:
	using widget_ptr = std::unique_ptr<widget>;

	place plc{ *this };
	std::vector<widget_ptr> widgets;

public:
	PreviewPanel(window& wd);
	virtual ~PreviewPanel();

	string parseRawDiv(const string& raw_div);
	void applyRawDiv(const string& raw_div);

	void refresh();

	template<typename T, typename ...Args>
	void addWidget(const string& tag, Args&& ... args) {
		static_assert(std::is_base_of<nana::widget, T>::value, "Type T in function addWidget needs to be a subclass of nana::widget.");

		T* t = new T(plc.window_handle(), std::forward<Args>(args)...);
		widget_ptr ptr(t);
		plc.field(tag.c_str()) << *t;
		widgets.push_back(std::move(ptr));
	}
};

