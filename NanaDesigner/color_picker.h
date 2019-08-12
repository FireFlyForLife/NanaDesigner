#pragma once
#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui.hpp>

namespace nana {
	class color_picker;

	struct arg_color_picker : public event_arg
	{
		color_picker& widget;
		arg_color_picker(color_picker&);
	};

	namespace drawerbase
	{
		namespace color_picker
		{
			struct color_picker_events : public general_events
			{
				basic_event<arg_color_picker> color_changed;
			};

			class drawer : public drawer_trigger
			{
				drawer(const drawer&) = delete;
				drawer(drawer&&) = delete;
				drawer& operator=(const drawer&) = delete;
				drawer& operator=(drawer&&) = delete;
			public:
				drawer();
//				~drawer();
//			private:
//				//Overrides drawer_trigger
//				void attached(widget_reference, graph_reference) override;
//				void detached() override;
//				void refresh(graph_reference)	override;
//
//				void focus(graph_reference, const arg_focus&)	override;
//				void mouse_wheel(graph_reference, const arg_wheel&) override;
//				void mouse_down(graph_reference, const arg_mouse&)	override;
//				void mouse_move(graph_reference, const arg_mouse&)	override;
//				void mouse_up(graph_reference, const arg_mouse& arg)	override;
//				void mouse_leave(graph_reference, const arg_mouse&)	override;
//				void key_press(graph_reference, const arg_keyboard&) override;
//				void key_char(graph_reference, const arg_keyboard&) override;
//				void resized(graph_reference, const arg_resized&) override;
			};
		}
	}
	class color_picker : public widget_object<category::widget_tag, drawerbase::color_picker::drawer, drawerbase::color_picker::color_picker_events>
	{
	public:
		color_picker();
		color_picker(window, bool visible);
		color_picker(window, const nana::rectangle& = {}, bool visible = true);
	};
}
