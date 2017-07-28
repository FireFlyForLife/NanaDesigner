#include "ResolutionForm.h"
#include <nana/gui/screen.hpp>
#include "Utils.h"
#include <iostream>


ResolutionForm::ResolutionForm(window owner, PreviewPanel& preview) : form(owner), preview(preview)
{
	caption("Specify preview size");
	size({ 300, 100 });

	//TODO: Make this a grid
	div(MULTILINE(
		vert 
		<max=25
			<fit lblWidth><fit min=120 txtWidth>
		>
		<max=25
			<fit lblHeight><fit min=120 txtHeight>
		>
	));

	lblWidth.text_align(align::right, align_v::center);
	lblHeight.text_align(align::right, align_v::center);
	txtHeight.caption(std::to_string(preview.size().height));
	txtWidth.caption(std::to_string(preview.size().width));
	txtHeight.events().text_changed([this](const arg_textbox& arg)
	{
		int num;
		try
		{
			num = atoi(arg.widget.caption().c_str());
			if (num > 0 && num < screen::desktop_size().height)
			{
				nana::size newsize = this->preview.size();
				newsize.height = num;
				this->preview.size(newsize);
			}
		}
		catch (std::exception e)
		{

		}


	});
	txtWidth.events().text_changed([this](const arg_textbox& arg)
	{
		int num;
		try
		{
			num = atoi(arg.widget.caption().c_str());
			if (num > 0 && num <= screen::desktop_size().width)
			{
				nana::size newsize = this->preview.size();
				newsize.width = num;
				this->preview.size(newsize);
			}
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}


	});

	(*this)["txtWidth"] << txtWidth;
	(*this)["lblWidth"] << lblWidth;
	(*this)["lblHeight"] << lblHeight;
	(*this)["txtHeight"] << txtHeight;
	collocate();
}
