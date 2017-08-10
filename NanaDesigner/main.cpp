#include <nana/gui.hpp>

#include "PreviewPanel.h"
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