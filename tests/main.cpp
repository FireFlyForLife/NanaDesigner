#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <iostream>

int main()
{
	using namespace nana;

	form fm;
	fm.caption("Hello World");
	textbox txt(fm, nana::rectangle(20, 20, 150, 30));
	txt.caption("Quit");
	//btn.events().click(API::exit_all);
	
	txt.set_highlight("div", colors::blue, colors::yellow);
	txt.set_keywords("div", false, false, { L"hi", L"Quit" });

	fm.show();
	exec();
}
