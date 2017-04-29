#include "PreviewPanel.h"



PreviewPanel::PreviewPanel(window& wd) : panel(wd)
{
}


PreviewPanel::~PreviewPanel()
{
}

string PreviewPanel::parseRawDiv(const string & raw_div)
{
	string div = raw_div;

	return div;
}

void PreviewPanel::applyRawDiv(const string& raw_div)
{
	//remove anything before and including '.div("' and everything inlcuding after the last '");'
	string div = parseRawDiv(raw_div);

	plc.div(div.c_str());
	plc.collocate();
}

void PreviewPanel::refresh()
{
	plc.collocate();
}
