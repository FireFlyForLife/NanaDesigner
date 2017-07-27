#include "PreviewPanel.h"
#include <iostream>


PreviewPanel::PreviewPanel(window& wd) : panel(wd)
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
	arg_resized sized;
	sized.window_handle = handle();
	sized.height = size().height;
	sized.width = size().width;
	events().resized.emit(sized, handle());
	
	plc.collocate();
}

int PreviewPanel::widgetAmount() const
{
	return widgets.size();
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
