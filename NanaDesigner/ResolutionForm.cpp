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
		<grid=[1,2] gap=5 margin=3 max=50 lbl>
		<grid=[1,2] gap=5 margin=3 min=120 txt>
	));

	lblWidth.text_align(align::right, align_v::center);
	lblHeight.text_align(align::right, align_v::center);

	txtHeight.caption(std::to_string(preview.size().height));
	txtHeight.multi_lines(false);
	txtHeight.set_accept([this](wchar_t key)
	{
		string text; 
		this->txtHeight.getline(0, text);
		return (key >= 48 && key <= 57 && text.size() < this->maxcharacters) || key == 8 || key == 46;
	});
	txtWidth.caption(std::to_string(preview.size().width));
	txtWidth.multi_lines(false);
	txtWidth.set_accept([this](wchar_t key)
	{
		std::cout << key << std::endl;
		string text;
		this->txtWidth.getline(0, text);
		return (key >= 48 && key <= 57 && text.size() < this->maxcharacters) || key == 8 || key == 46;
	});

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

	(*this)["txt"] << txtWidth;
	(*this)["lbl"] << lblWidth;
	(*this)["lbl"] << lblHeight;
	(*this)["txt"] << txtHeight;
	collocate();
}
