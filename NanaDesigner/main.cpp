#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/group.hpp>

#include <nana/gui/msgbox.hpp>
#include <nana/gui/filebox.hpp>
#include <thread>

#include "WidgetAdder.h"
#include "PreviewPanel.h"

#include <string>
#include <iostream>
#include "DesignerForm.h"

using namespace nana;
using namespace std;

DesignerForm* main_form = nullptr;

int main()
{
	using namespace nana;
	using namespace std;

	main_form = new DesignerForm();

	PreviewPanel* preview = main_form->GetPreviewPanel();
	preview->applyRawDiv("<><btn>");
	preview->addWidget<button>("btn", "Buttaaan!");
	main_form->Refresh();

	main_form->show();

	exec();

	delete main_form;
}