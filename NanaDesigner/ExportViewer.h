#pragma once
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/system/dataexch.hpp>
#include "PreviewPanel.h"
#include "json.hpp"
#include "nana_typeinfo.h"

using namespace nana;
using json = nlohmann::json;

class ExportViewer : public form
{
public:
	ExportViewer(window window_handle);

	void GenerateCode(PreviewPanel& preview);
	static json GenerateCodeContents(PreviewPanel& preview);

protected:
	label headerLabel{ *this, "Header file:" };
	button headerCopy{ *this, "Copy to Clipboard" };
	textbox headerText{ *this };

	label sourceLabel{ *this, "Source file:" };
	button sourceCopy{ *this, "Copy to Clipboard" };
	textbox sourceText{ *this };

	system::dataexch clipboard;
};
