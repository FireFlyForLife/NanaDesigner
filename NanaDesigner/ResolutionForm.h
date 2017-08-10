#pragma once
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include "PreviewPanel.h"
#include <nana/gui/widgets/button.hpp>


using namespace nana;

class ResolutionForm : public form {
public:
	const int maxcharacters = 5;
	const nana::size minPreviewSize = { 20, 30 };
public:
	ResolutionForm(window owner, PreviewPanel& preview);
	~ResolutionForm();

protected:
	textbox txtWidth{ *this, "900" };
	label lblWidth{ *this, "Width:" };
	label lblHeight{ *this, "Height:" };
	textbox txtHeight{ *this, "1080" };

	int8_t ignore_textchange;
	bool was_floating;

	PreviewPanel& preview;
private:
	event_handle container_resizing_handle = nullptr;
};