#include "PreviewPanel.h"
#include <iostream>


PreviewPanel::PreviewPanel(window wd) : panel<true>(wd)
{
	
}


PreviewPanel::~PreviewPanel()
{
}

string PreviewPanel::parseRawDiv(const string & raw_div) const
{
	string div = raw_div;
	
	//remove anything before and including '.div("'
	static string start = ".div(\"";
	auto startpos = div.find(start);
	if (startpos != string::npos) {
		div.erase(0, startpos + start.size());
	}

	//and everything inlcuding after the last '");'
	static string end = "\");";
	auto endpos = div.rfind(end);
	if(endpos != string::npos)
	{
		div.erase(endpos, endpos + end.size());
	}

	return div;
}

void PreviewPanel::applyDiv(const string& div)
{
	plc.div(div.c_str());
	divStr = div;

	plc.collocate();
}

void PreviewPanel::applyRawDiv(const string& raw_div)
{
	string div = parseRawDiv(raw_div);

	applyDiv(div);
}

string PreviewPanel::getDiv() const
{
	return divStr;
}

void PreviewPanel::refresh()
{	
	plc.collocate();
}

int PreviewPanel::widgetAmount() const
{
	return widgets.size();
}

void PreviewPanel::addWidget(widget_pair& pair)
{
	plc.field(pair.first.c_str()) << *pair.second;
	widgets.push_back(pair);
}

void PreviewPanel::removeWidget(widget_pair& pair)
{

	for (int i = 0; i < widgets.size(); ++i)
	{
		if(pair == widgets[i])
		{
			removeWidget(i);
			return;
		}
	}
}

void PreviewPanel::removeWidget(int index)
{
	widget_pair& pair = widgets[index];

	plc.field_display(pair.first.c_str(), false);
	plc.erase(*pair.second); //TODO: report that just erasing a window doesn't fully clear the lower and right part of the image.
	plc.field_display(pair.first.c_str(), true);
	widgets.erase(widgets.begin() + index);
}

PreviewPanel::widget_pair& PreviewPanel::getWidget(int index)
{
	return widgets[index];
}

widget* PreviewPanel::getDockarea() const
{
	window dockarea = parent();
	window dockarea_container = API::get_parent_window(dockarea);
	string cap = API::window_caption(dockarea_container);
	std::cout << cap << std::endl;
	bool is_floating = cap == "Nana Window";
	widget* dockwidget = API::get_widget(is_floating ? dockarea_container : dockarea);

	return dockwidget;
}

bool PreviewPanel::isFloating() const
{
	window dockarea = parent();
	window dockarea_container = API::get_parent_window(dockarea);
	string cap = API::window_caption(dockarea_container);

	return cap == "Nana Window";
}

//void PreviewPanel::size(const nana::size& newsize)
//{
//	std::cout << "the new size method!" << std::endl;
//	API::move_window(this->parent(), rectangle{ point{0,0}, newsize });
//}
//
//nana::size PreviewPanel::size() const
//{
//	return panel<true>::size();
//}
