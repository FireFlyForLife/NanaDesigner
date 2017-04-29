#include "PreviewPanel.h"


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

void PreviewPanel::applyRawDiv(const string& raw_div)
{
	string div = parseRawDiv(raw_div);

	plc.div(div.c_str());
	plc.collocate();
}

void PreviewPanel::refresh()
{
	plc.collocate();
}
