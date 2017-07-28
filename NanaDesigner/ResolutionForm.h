#pragma once
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include "PreviewPanel.h"


using namespace nana;

class ResolutionForm : public form {
public:
	ResolutionForm(window owner, PreviewPanel& preview);

protected:
	textbox txtWidth{ *this, "900" };
	label lblWidth{ *this, "Width:" };
	label lblHeight{ *this, "Height:" };
	textbox txtHeight{ *this, "1080" };

	PreviewPanel& preview;
};