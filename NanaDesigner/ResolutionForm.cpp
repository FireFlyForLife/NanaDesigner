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

	was_floating = false;
	ignore_textchange = 0;

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
		if (ignore_textchange > 0)
			ignore_textchange--;
		else {
			int num;
			try
			{
				num = atoi(arg.widget.caption().c_str());
				if (num > 0 && num < screen::desktop_size().height)
				{
					widget* dockarea = this->preview.getDockarea();
					nana::size newsize = dockarea->size();
					newsize.height = fmax(num, minPreviewSize.height);
					dockarea->size(newsize);
				}
			}
			catch (std::exception e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	});
	txtWidth.events().text_changed([this](const arg_textbox& arg)
	{
		if (ignore_textchange > 0)
			ignore_textchange--;
		else {
			int num;
			try
			{
				num = atoi(arg.widget.caption().c_str());
				if (num > 0 && num <= screen::desktop_size().width)
				{
					widget* dockarea = this->preview.getDockarea();
					nana::size newsize = dockarea->size();
					newsize.width = fmax(num, minPreviewSize.width);
					dockarea->size(newsize);
				}
			}
			catch (std::exception e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	});
	
//	preview.getDockarea()->events().destroy([this](const arg_destroy& arg)
//	{
//		//unregister events
//		std::cout << "dockarea destroyed" << std::endl;
//	});
//	preview.getDockarea()->events().resizing([this](const arg_resizing& arg)
//	{
//		std::cout << "resizing container:{" << arg.width << "," << arg.height << "}" << static_cast<int>(arg.border) << std::endl;
//	});
	preview.events().resizing([this](const arg_resizing& arg)
	{
		
		bool isFloating = this->preview.isFloating();
		std::cout << this->was_floating << " " << isFloating << std::endl;
		if(!this->was_floating && isFloating)
		{
			this->was_floating = true;
			//register events
			auto dockarea = this->preview.getDockarea();
			container_resizing_handle = dockarea->events().resizing([this](const arg_resizing& arg)
			{
				if(arg.border != window_border::none)
				{
					auto s = this->size();
					ignore_textchange += (arg.width != s.width);
					ignore_textchange += (arg.height != s.height);
					this->txtWidth.caption(std::to_string(arg.width));
					this->txtHeight.caption(std::to_string(arg.height));
				}
			});
			dockarea->events().destroy([this, dockarea](const arg_destroy& arg)
			{
				//unregister events //TODO: is this neccesairy?
				if(container_resizing_handle)
				{
					dockarea->events().resizing.remove(container_resizing_handle);
					container_resizing_handle = nullptr;
				}

				//reset variables
				was_floating = false;
			});
		}
		
		//std::cout << "resizing:{" << arg.width << "," << arg.height << "}" << static_cast<int>(arg.border) << std::endl;
	});


	(*this)["txt"] << txtWidth;
	(*this)["lbl"] << lblWidth;
	(*this)["lbl"] << lblHeight;
	(*this)["txt"] << txtHeight;
	collocate();
}

ResolutionForm::~ResolutionForm()
{
}
