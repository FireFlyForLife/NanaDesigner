#pragma once
#include <nana/gui.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/place.hpp>

#include <vector>
#include <memory>

using namespace nana;
using std::string;
using std::shared_ptr;

class PreviewPanel : public panel<true>
{
public:
	using widget_ptr = std::shared_ptr<widget>;
	using widget_pair = std::pair<string, widget_ptr>;

	template<typename T>
	struct placed_widget
	{
		string tag;
		widget_ptr widget = nullptr;

		placed_widget();
		placed_widget(string tag, widget_ptr widget);

		
	};

	PreviewPanel(window wd);
	virtual ~PreviewPanel();

	string parseRawDiv(const string& raw_div) const;
	void applyDiv(const string& div);
	void applyRawDiv(const string& raw_div);

	string getDiv() const;

	void refresh();

	int widgetAmount() const;

	template<typename T, typename ...Args>
	shared_ptr<T> addWidget(const string& tag, Args&& ... args) {
		static_assert(std::is_base_of<nana::widget, T>::value, "Type T in function addWidget needs to be a subclass of nana::widget.");

		T* t = new T(plc.window_handle(), std::forward<Args>(args)...);
		shared_ptr<T> ptr(t);

		plc.field(tag.c_str()) << *ptr;

		widget_pair pair{ tag, ptr };
		widgets.push_back(pair);
		return ptr;
	}

	void addWidget(widget_pair& pair);

	void removeWidget(widget_pair& pair);
	void removeWidget(int index);

	widget_pair& getWidget(int index);

	widget* getDockarea() const;
	bool isFloating() const;

//	void size(const nana::size& newsize);
//	nana::size size() const;

protected:
	place plc{ *this };
	string divStr;

	std::vector<widget_pair> widgets;
};

