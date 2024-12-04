namespace gui { // Zerogui base
	namespace defines {
		float offset_y = 0.f;
		float offset_x = 0.f;
		float pushYvalue = 0.f;
		float pushXvalue = 0.f;

		int current_element = -1;
		int elements_count = 0;

		fvector2d drag_position = {};
		fvector2d menu_position = {};
		fvector2d first_element_pos = {};
		fvector2d current_element_pos = {};
		fvector2d current_element_size = {};
		fvector2d last_element_pos = {};
		fvector2d last_element_size = {};
		fvector2d window_position = { 100, 100 };
		fvector2d window_size = { 465, 450 };

		bool is_left_mouse_down = false; // ?
		bool same_line = false;
		bool pushY = false;
		bool pushX = false;
		bool gover_element = false;
		bool element_pressed = false; // ?
		bool cursor_toggled = false;
		bool hover_element = false;

		canvas* _canvas;
	}



	namespace colors {
		flinearcolor background_color = { 1.6, 1.6, 2.6, 255 };
		flinearcolor header_color = { 3, 94, 252, 255 };

		flinearcolor tab_button_active = { 3, 94, 252, 255 };
		flinearcolor tab_button_hovered = { 99, 99, 99, 255 };
		flinearcolor tab_button_idle = { 64, 64, 64, 255 };

		flinearcolor button_active = { 3, 94, 252, 255 };
		flinearcolor button_hovered = { 99, 99, 99, 255 };
		flinearcolor button_idle = { 64, 64, 64, 255 };

		flinearcolor checkbox_hovered = { 14.199991, 14.999990, 21.200008, 255 };
		flinearcolor checkbox_enabled = { 3, 94, 252, 255 };
		flinearcolor checkbox_idle = { 7.599998, 7.999997, 13.199992, 255 };

		flinearcolor slider_hovered = { 14.199991, 14.999990, 21.200008, 255 };
		flinearcolor slider_progress = { 3, 94, 252, 255 };
		flinearcolor slider_idle = { 7.599998, 7.999997, 13.199992, 255 };
		flinearcolor slider_button = { 3, 94, 252, 255 };

		flinearcolor combobox_hovered = { 14.199991, 14.999990, 21.200008, 255 };
		flinearcolor combobox_idle = { 7.599998, 7.999997, 13.199992, 255 };

		flinearcolor colorpicker_background = { 7.599998, 7.999997, 13.199992, 255 };
	}

	namespace input_handler {
		namespace defines {
			bool mouseDown = false;
			bool mouseDownAlready[256];
		}

		bool is_mouse_clicked(int element_id, bool repeat) {
			if (defines::mouseDown) {
				if (!defines::mouseDownAlready[element_id]) {
					defines::mouseDownAlready[element_id] = true;
					return true;
				}

				if (repeat) return true;
			}
			else defines::mouseDownAlready[element_id] = false;

			return false;
		}

		void run(controller* controller) {
			defines::mouseDown = controller->is_input_key_down(pinguu::keys::left_mouse_button);
		}
	}

	namespace functions {
		wchar_t* s2wc(const char* c) {
			const size_t cSize = strlen(c) + 1;
			wchar_t* wc = new wchar_t[cSize];
			mbstowcs(wc, c, cSize);
			return wc;
		}

		void draw_line(fvector2d from, fvector2d to, int thickness, flinearcolor color) {
			gui::defines::_canvas->draw_line(fvector2d{ from.x, from.y }, fvector2d{ to.x, to.y }, thickness, color);
		}

		void draw_filled_rect(fvector2d pos, float width, float height, flinearcolor color) {
			for (int i = 0; i < height; i++)
				gui::defines::_canvas->draw_line({ pos.x, pos.y + i }, { pos.x + width, pos.y + i }, 1.f, { color.red, color.green, color.blue, 1 });
		}

		fvector2d get_cursor_position() {
			float x = 0.f, y = 0.f;
			if (controller* controller = blueprint::get_first_local_player_controller(globals::get_world_instance()))
				controller->get_mouse_position(x, y);

			return { x, y };
		}

		bool is_mouse_in_zone(fvector2d pos, fvector2d size) {
			fvector2d cursor_position = functions::get_cursor_position();

			if (cursor_position.x > pos.x && cursor_position.y > pos.y)
				if (cursor_position.x < pos.x + size.x && cursor_position.y < pos.y + size.y)
					return true;

			return false;
		}

		void toggle_cursor(bool status) {
			defines::cursor_toggled = status;

			if (defines::cursor_toggled) {
				fvector2d cursor_position = functions::get_cursor_position();
				defines::_canvas->draw_line({ cursor_position.x, cursor_position.y }, { cursor_position.x + 35, cursor_position.y + 10 }, 1.f, { 0.80f, 0.30f, 0.30f, 1.0f });

				int x = 35, y = 10;
				while (y != 30) {
					x -= 1; if (x < 15) x = 15;
					y += 1; if (y > 30) y = 30;

					defines::_canvas->draw_line({ cursor_position.x, cursor_position.y }, { cursor_position.x + x, cursor_position.y + y }, 1.f, { 0.80f, 0.30f, 0.30f, 1.0f });
				}

				defines::_canvas->draw_line({ cursor_position.x, cursor_position.y }, { cursor_position.x + 15, cursor_position.y + 30 }, 1.f, { 0.80f, 0.30f, 0.30f, 1.0f });
				defines::_canvas->draw_line({ cursor_position.x + 35, cursor_position.y + 10 }, { cursor_position.x + 15, cursor_position.y + 30 }, 1.f, { 0.80f, 0.30f, 0.30f, 1.0f });
			}
		}

		void same_line() {
			defines::same_line = true;
		}

		void push_next_element_y(float y, bool from_last_element = true) {
			defines::pushY = true;

			if (from_last_element) defines::pushYvalue = defines::last_element_pos.y + defines::last_element_size.y + y;
			else defines::pushYvalue = defines::menu_position.y + y;
		}

		void push_next_element_x(float x, bool from_last_element = true) {
			defines::pushX = true;

			if (from_last_element) defines::pushXvalue = defines::last_element_pos.x + defines::last_element_size.x + x;
			else defines::pushXvalue = defines::menu_position.x + x;
		}

		void next_column(float x) {
			defines::offset_x = x;
			functions::push_next_element_y(defines::first_element_pos.y, false);
		}

		void clear_first_pos() {
			defines::first_element_pos = {};
		}

		void text_left(fstring text, fvector2d pos, float size, flinearcolor color, bool outline, uintptr_t* font = RobotoRegular) {
			defines::_canvas->draw_text((::uobject*)font, text, pos, { size, size }, color, false, {}, { pos.x + 1, pos.y + 1 }, false, false, outline, { 0,0,0,1 });
		}

		void text_center(fstring text, fvector2d pos, float size, flinearcolor color, bool outline, uintptr_t* font = RobotoRegular) {
			defines::_canvas->draw_text((::uobject*)font, text, pos, { size, size }, color, false, {}, { pos.x + 1, pos.y + 1 }, true, true, outline, { 0,0,0,1 });
		}
	}

	namespace important_draw {
		struct draw_list
		{
			int type = -1; //1 = FilledRect, 2 = TextLeft, 3 = TextCenter, 4 = Draw_Line
			fvector2d pos;
			fvector2d size;
			flinearcolor color;
			float scale = 1.f;
			char* name;
			bool outline;

			fvector2d from;
			fvector2d to;
			int thickness;

			uintptr_t* font;
		};
		draw_list drawlist[255];

		void draw_filled_rect(fvector2d pos, float width, float height, flinearcolor color) {
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 1;
					drawlist[i].pos = pos;
					drawlist[i].size = { width, height };
					drawlist[i].color = color;
					return;
				}
			}
		}

		void text_left(char* text, fvector2d pos, float size, flinearcolor color, bool outline, uintptr_t* font = RobotoRegular) {
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 2;
					drawlist[i].name = text;
					drawlist[i].scale = size;
					drawlist[i].font = font;
					drawlist[i].pos = pos;
					drawlist[i].outline = outline;
					drawlist[i].color = color;
					return;
				}
			}
		}

		void text_center(char* text, fvector2d pos, float size, flinearcolor color, bool outline, uintptr_t* font = RobotoRegular) {
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 3;
					drawlist[i].name = text;
					drawlist[i].scale = size;
					drawlist[i].font = font;
					drawlist[i].pos = pos;
					drawlist[i].outline = outline;
					drawlist[i].color = color;
					return;
				}
			}
		}

		void draw_line(fvector2d from, fvector2d to, int thickness, flinearcolor color)
		{
			for (int i = 0; i < 128; i++)
			{
				if (drawlist[i].type == -1)
				{
					drawlist[i].type = 4;
					drawlist[i].from = from;
					drawlist[i].to = to;
					drawlist[i].thickness = thickness;
					drawlist[i].color = color;
					return;
				}
			}
		}
	}

	void important_renderer()
	{
		for (int i = 0; i < 128; i++)
		{
			if (important_draw::drawlist[i].type != -1)
			{
				//Filled Rect
				if (important_draw::drawlist[i].type == 1)
				{
					gui::functions::draw_filled_rect(important_draw::drawlist[i].pos, important_draw::drawlist[i].size.x, important_draw::drawlist[i].size.y, important_draw::drawlist[i].color);
				}
				//TextLeft
				else if (important_draw::drawlist[i].type == 2)
				{
					std::string name_string = important_draw::drawlist[i].name;
					std::wstring name_wstring(name_string.begin(), name_string.end());

					gui::functions::text_left(name_wstring.c_str(), important_draw::drawlist[i].pos, important_draw::drawlist[i].scale, important_draw::drawlist[i].color, important_draw::drawlist[i].outline);
				}
				//TextCenter
				else if (important_draw::drawlist[i].type == 3)
				{
					std::string name_string = important_draw::drawlist[i].name;
					std::wstring name_wstring(name_string.begin(), name_string.end());

					gui::functions::text_center(name_wstring.c_str(), important_draw::drawlist[i].pos, important_draw::drawlist[i].scale, important_draw::drawlist[i].color, important_draw::drawlist[i].outline);
				}
				//Draw_Line
				else if (important_draw::drawlist[i].type == 4)
				{
					functions::draw_line(important_draw::drawlist[i].from, important_draw::drawlist[i].to, important_draw::drawlist[i].thickness, important_draw::drawlist[i].color);
				}

				important_draw::drawlist[i].type = -1;
			}
		}
	}

	bool window(fvector2d* pos, fvector2d size, bool is_open) {
		defines::elements_count = 0;

		if (!is_open) return false;

		if (controller* controller = blueprint::get_first_local_player_controller(globals::get_world_instance())) {
			bool is_hovered = functions::is_mouse_in_zone({ pos->x, pos->y }, size);

			input_handler::run(controller);

			if (defines::current_element != -1 && !controller->is_input_key_down(pinguu::keys::left_mouse_button))
				defines::current_element = -1;


			if ((is_hovered || defines::drag_position.x != 0) && !defines::hover_element)
			{
				if (input_handler::is_mouse_clicked(defines::elements_count, true))
				{
					fvector2d cursor_position = functions::get_cursor_position();

					cursor_position -= size;

					if (defines::drag_position.x == 0) {
						defines::drag_position.x = (cursor_position.x - pos->x);
						defines::drag_position.y = (cursor_position.y - pos->y);
					}

					pos->x = cursor_position.x - defines::drag_position.x;
					pos->y = cursor_position.y - defines::drag_position.y;
				}
				else defines::drag_position = {};
			}
			else defines::hover_element = false; // ?

			defines::offset_x = 0.0f;
			defines::offset_y = 0.0f;
			defines::menu_position = { pos->x, pos->y };
			defines::first_element_pos = { 0, 23 };
			defines::last_element_pos = { 0, 0 };
			defines::last_element_size = { 0, 0 };
			defines::current_element_pos = { 0,0 };
			defines::current_element_size = { 0,0 };

			defines::offset_y += 10.f;

			functions::draw_filled_rect({ pos->x, pos->y }, size.x, size.y, colors::background_color.real()); // Background

			functions::draw_filled_rect({ pos->x, pos->y }, size.x, 5.f, colors::header_color.real()); // Header
			functions::draw_filled_rect({ pos->x, pos->y + size.y }, size.x, 5.f, colors::header_color.real()); // Header


			return true;
		}

		return false;
	}

	void text(fstring text, float scale = 1.f, bool center = false, bool outline = false, float x = 0, float y = 0) {
		defines::elements_count++;

		float size = 25;

		fvector2d padding = { 10, 10 };
		fvector2d position = { defines::menu_position.x + padding.x + defines::offset_x, defines::menu_position.y + defines::offset_y };

		if (defines::same_line) {
			position.x = defines::last_element_pos.x + defines::last_element_size.x + padding.x;
			position.y = defines::last_element_pos.y;
		}

		if (defines::pushY) {
			position.y = defines::pushYvalue;
			defines::pushY = false;
			defines::pushYvalue = 0.0f;
			defines::offset_y = position.y - defines::menu_position.y;
		}

		if (defines::pushX) {
			position.x = defines::pushXvalue;
			defines::pushX = false;
			defines::pushXvalue = 0.0f;
			defines::offset_x = position.x - defines::menu_position.x;
		}

		if (!defines::same_line)
			defines::offset_y += size + padding.y;

		fvector2d text_position = { position.x + x, position.y + size / 2 + y };
		if (center) functions::text_center(text, text_position, scale, { 1,1,1,1 }, outline);
		else functions::text_left(text, text_position, scale, { 1,1,1,1 }, outline);

		defines::same_line = false;
		defines::last_element_pos = position;

		if (defines::first_element_pos.x == 0.0f) defines::first_element_pos = position;
		else defines::last_element_pos = position;
	}

	bool button_tab(fstring name, fvector2d size, float correction_x = 0, float correction_y = 0, bool active = false) {
		defines::elements_count++;

		fvector2d padding = { 5, 10 };
		fvector2d position = { defines::menu_position.x + padding.x + defines::offset_x, defines::menu_position.y + padding.y + defines::offset_y };

		if (defines::same_line) {
			position.x = defines::last_element_pos.x + defines::last_element_size.x + padding.x;
			position.y = defines::last_element_pos.y;
		}

		if (defines::pushY) {
			position.y = defines::pushYvalue;
			defines::pushY = false;
			defines::pushYvalue = 0.0f;
			defines::offset_y = position.y - defines::menu_position.y;
		}

		if (defines::pushX) {
			position.x = defines::pushXvalue;
			defines::pushX = false;
			defines::pushXvalue = 0.0f;
			defines::offset_x = position.x - defines::menu_position.x;
		}

		bool is_hovered = functions::is_mouse_in_zone(position, size);

		if (active) functions::draw_filled_rect(position, size.x, size.y, colors::tab_button_active.real());
		else if (is_hovered) functions::draw_filled_rect(position, size.x, size.y, colors::tab_button_hovered.real()), defines::hover_element = true;
		else functions::draw_filled_rect(position, size.x, size.y, colors::tab_button_idle.real());

		if (!defines::same_line)
			defines::offset_y += size.y + padding.y;

		fvector2d text_position = { (position.x + size.x / 2) - correction_x, (position.y + size.y / 2) - correction_y };
		functions::text_center(name, text_position, 0.85f, active ? flinearcolor(1, 1, 1, 1) : flinearcolor(0.7, 0.7, 0.7, 1), false, RobotoBold);

		defines::same_line = false;
		defines::last_element_pos = position;
		defines::last_element_size = size;

		if (defines::first_element_pos.x = 0.0f) defines::first_element_pos = position;

		if (controller* controller = blueprint::get_first_local_player_controller(globals::get_world_instance()))
			if (is_hovered && controller->was_input_key_just_pressed(pinguu::keys::left_mouse_button))
				return true;

		return false;
	}

	bool button_sub_tab(fstring name, fvector2d size, bool active = false) {
		defines::elements_count++;

		fvector2d padding = { 5, 10 };
		fvector2d position = { defines::menu_position.x + padding.x + defines::offset_x, defines::menu_position.y + padding.y + defines::offset_y };

		if (defines::same_line) {
			position.x = defines::last_element_pos.x + defines::last_element_size.x + padding.x;
			position.y = defines::last_element_pos.y;
		}

		if (defines::pushY) {
			position.y = defines::pushYvalue;
			defines::pushY = false;
			defines::pushYvalue = 0.0f;
			defines::offset_y = position.y - defines::menu_position.y;
		}

		if (defines::pushX) {
			position.x = defines::pushXvalue;
			defines::pushX = false;
			defines::pushXvalue = 0.0f;
			defines::offset_x = position.x - defines::menu_position.x;
		}

		bool is_hovered = functions::is_mouse_in_zone(position, size);

		if (active) functions::draw_filled_rect(position, size.x, size.y, colors::tab_button_active.real());
		else if (is_hovered) functions::draw_filled_rect(position, size.x, size.y, colors::tab_button_hovered.real()), defines::hover_element = true;
		else functions::draw_filled_rect(position, size.x, size.y, colors::tab_button_idle.real());

		if (!defines::same_line)
			defines::offset_y += size.y + padding.y;

		fvector2d text_position = { (position.x + size.x / 2), (position.y + size.y / 2) };
		functions::text_center(name, text_position, 0.8f, { 0.10, 0.10, 0.10, 1 }, false, RobotoRegular);

		defines::same_line = false;
		defines::last_element_pos = position;
		defines::last_element_size = size;

		if (defines::first_element_pos.x = 0.0f) defines::first_element_pos = position;

		if (controller* controller = blueprint::get_first_local_player_controller(globals::get_world_instance()))
			if (is_hovered && controller->was_input_key_just_pressed(pinguu::keys::left_mouse_button))
				return true;

		return false;
	}

	bool button(fstring name, fvector2d size) {
		defines::elements_count++;

		fvector2d padding = { 5, 10 };
		fvector2d position = { defines::menu_position.x + padding.x + defines::offset_x, defines::menu_position.y + padding.y + defines::offset_y };

		if (defines::same_line) {
			position.x = defines::last_element_pos.x + defines::last_element_size.x + padding.x;
			position.y = defines::last_element_pos.y;
		}

		if (defines::pushY) {
			position.y = defines::pushYvalue;
			defines::pushY = false;
			defines::pushYvalue = 0.0f;
			defines::offset_y = position.y - defines::menu_position.y;
		}

		if (defines::pushX) {
			position.x = defines::pushXvalue;
			defines::pushX = false;
			defines::pushXvalue = 0.0f;
			defines::offset_x = position.x - defines::menu_position.x;
		}

		bool is_hovered = functions::is_mouse_in_zone(position, size);

		if (is_hovered) functions::draw_filled_rect(position, size.x, size.y, colors::button_hovered.real()), defines::hover_element = true;
		else functions::draw_filled_rect(position, size.x, size.y, colors::button_idle.real());

		if (!defines::same_line)
			defines::offset_y += size.y + padding.y;

		fvector2d text_position = { (position.x + size.x / 2), (position.y + size.y / 2) };
		functions::text_center(name, text_position, 0.8f, { 1, 1, 1, 1 }, false);

		defines::same_line = false;
		defines::last_element_pos = position;
		defines::last_element_size = size;

		if (defines::first_element_pos.x = 0.0f) defines::first_element_pos = position;

		if (controller* controller = blueprint::get_first_local_player_controller(globals::get_world_instance()))
			if (is_hovered && controller->was_input_key_just_pressed(pinguu::keys::left_mouse_button))
				return true;

		return false;
	}

	void checkbox(fstring name, float correction_y, bool* value, flinearcolor color = { 1, 1, 1, 1 }) {
		defines::elements_count++;

		float size = 15;

		fvector2d padding = { 10, 5 };
		fvector2d position = { defines::menu_position.x + padding.x + defines::offset_x, defines::menu_position.y + padding.y + defines::offset_y };

		if (defines::same_line) {
			position.x = defines::last_element_pos.x + defines::last_element_size.x + padding.x;
			position.y = defines::last_element_pos.y;
		}

		if (defines::pushY) {
			position.y = defines::pushYvalue;
			defines::pushY = false;
			defines::pushYvalue = 0.0f;
			defines::offset_y = position.y - defines::menu_position.y;
		}

		if (defines::pushX) {
			position.x = defines::pushXvalue;
			defines::pushX = false;
			defines::pushXvalue = 0.0f;
			defines::offset_x = position.x - defines::menu_position.x;
		}

		bool is_hovered = functions::is_mouse_in_zone(position, { size, size });

		if (is_hovered) functions::draw_filled_rect(position, size, size, colors::checkbox_hovered.real()), defines::hover_element = true;
		else functions::draw_filled_rect(position, size, size, colors::checkbox_idle.real());

		if (!defines::same_line)
			defines::offset_y += size + padding.y;

		if (*value) functions::draw_filled_rect({ position.x + 2, position.y + 2 }, size - 4, size - 4, colors::checkbox_enabled.real());

		fvector2d text_position = { position.x + size + 10.f, (position.y + size / 2) - correction_y };
		functions::text_left(name, text_position, 0.9f, color, false);

		defines::same_line = false;
		defines::last_element_pos = position;

		if (defines::first_element_pos.x = 0.0f) defines::first_element_pos = position;

		if (controller* controller = blueprint::get_first_local_player_controller(globals::get_world_instance()))
			if (is_hovered && controller->was_input_key_just_pressed(pinguu::keys::left_mouse_button))
				*value = !*value;
	}

	void same_line() {
		defines::same_line = true;
	}

	void slider_int(fstring name, int* value, int min, int max, std::string result = "%d") {
		defines::elements_count++;

		fvector2d size = fvector2d{ 340, 50 };
		fvector2d slider_size = fvector2d{ 300, 12 };

		fvector2d padding = { 10, 10 };
		fvector2d position = { defines::menu_position.x + padding.x + defines::offset_x, defines::menu_position.y + padding.y + defines::offset_y };

		if (defines::same_line) {
			position.x = defines::last_element_pos.x + defines::last_element_size.x + padding.x;
			position.y = defines::last_element_pos.y;
		}

		if (defines::pushY) {
			position.y = defines::pushYvalue;
			defines::pushY = false;
			defines::pushYvalue = 0.0f;
			defines::offset_y = position.y - defines::menu_position.y;
		}

		if (defines::pushX) {
			position.x = defines::pushXvalue;
			defines::pushX = false;
			defines::pushXvalue = 0.0f;
			defines::offset_x = position.x - defines::menu_position.x;
		}

		bool is_hovered = functions::is_mouse_in_zone({ position.x, position.y + slider_size.y + padding.y }, slider_size);

		if (!defines::same_line)
			defines::offset_y += size.y + padding.y;

		if (is_hovered || defines::current_element == defines::elements_count) {
			if (controller* controller = blueprint::get_first_local_player_controller(globals::get_world_instance())) {
				if (input_handler::is_mouse_clicked(defines::elements_count, true)) {
					defines::current_element = defines::elements_count;

					fvector2d cursor_position = functions::get_cursor_position();

					*value = ((cursor_position.x - position.x) * ((max - min) / slider_size.x)) + min;
					if (*value < min) *value = min;
					if (*value > max) *value = max;
				}

				functions::draw_filled_rect({ position.x, position.y + slider_size.y + padding.y }, slider_size.x, slider_size.y, colors::slider_hovered.real());
				functions::draw_filled_rect({ position.x, position.y + slider_size.y + padding.y + 5.f }, 5.f, 5.f, colors::slider_progress.real());

				defines::hover_element = true;
			}
		}
		else {
			functions::draw_filled_rect({ position.x, position.y + slider_size.y + padding.y }, slider_size.x, slider_size.y, colors::slider_idle.real());
			functions::draw_filled_rect({ position.x, position.y + slider_size.y + padding.y + 5.f }, 5.f, 5.f, colors::slider_progress.real());
		}

		float oneP = slider_size.x / (max - min);

		functions::draw_filled_rect({ position.x, position.y + slider_size.y + padding.y }, oneP * (*value - min), slider_size.y, colors::slider_progress.real());
		functions::draw_filled_rect({ position.x + oneP * (*value - min), position.y + slider_size.y + padding.y - 3.f }, 10.f, slider_size.y + 6.f, colors::slider_button.real());

		char value_test[64];
		sprintf(value_test, result.c_str(), *value);

		std::string value_string = value_test;
		std::wstring long_value(value_string.begin(), value_string.end());

		fvector2d value_position = { position.x + size.x - 34, position.y + 20 };
		functions::text_left(long_value.c_str(), value_position, 0.8f, { 1, 1, 1, 1 }, false);

		fvector2d text_position = { position.x - 2, position.y - 2 };
		functions::text_left(name, text_position, 0.8f, { 1, 1, 1, 1 }, false);

		defines::same_line = false;
		defines::last_element_pos = position;
		defines::last_element_size = size;

		if (defines::first_element_pos.x = 0.0f) defines::first_element_pos = position;
	}

	void slider_float(fstring name, float* value, float min, float max, std::string result = "%.2f%") {
		defines::elements_count++;

		fvector2d size = fvector2d{ 340, 50 };
		fvector2d slider_size = fvector2d{ 300, 12 };

		fvector2d padding = { 10, 10 };
		fvector2d position = { defines::menu_position.x + padding.x + defines::offset_x, defines::menu_position.y + padding.y + defines::offset_y };

		if (defines::same_line) {
			position.x = defines::last_element_pos.x + defines::last_element_size.x + padding.x;
			position.y = defines::last_element_pos.y;
		}

		if (defines::pushY) {
			position.y = defines::pushYvalue;
			defines::pushY = false;
			defines::pushYvalue = 0.0f;
			defines::offset_y = position.y - defines::menu_position.y;
		}

		if (defines::pushX) {
			position.x = defines::pushXvalue;
			defines::pushX = false;
			defines::pushXvalue = 0.0f;
			defines::offset_x = position.x - defines::menu_position.x;
		}

		bool is_hovered = functions::is_mouse_in_zone({ position.x, position.y + slider_size.y + padding.y }, slider_size);

		if (!defines::same_line)
			defines::offset_y += size.y + padding.y;

		if (is_hovered || defines::current_element == defines::elements_count) {
			if (controller* controller = blueprint::get_first_local_player_controller(globals::get_world_instance())) {
				if (input_handler::is_mouse_clicked(defines::elements_count, true)) {
					defines::current_element = defines::elements_count;

					fvector2d cursor_position = functions::get_cursor_position();

					*value = ((cursor_position.x - position.x) * ((max - min) / slider_size.x)) + min;
					if (*value < min) *value = min;
					if (*value > max) *value = max;
				}

				functions::draw_filled_rect({ position.x, position.y + slider_size.y + padding.y }, slider_size.x, slider_size.y, colors::slider_hovered.real());
				functions::draw_filled_rect({ position.x, position.y + slider_size.y + padding.y + 5.f }, 5.f, 5.f, colors::slider_progress.real());

				defines::hover_element = true;
			}
		}
		else {
			functions::draw_filled_rect({ position.x, position.y + slider_size.y + padding.y }, slider_size.x, slider_size.y, colors::slider_idle.real());
			functions::draw_filled_rect({ position.x, position.y + slider_size.y + padding.y + 5.f }, 5.f, 5.f, colors::slider_progress.real());
		}

		float oneP = slider_size.x / (max - min);

		functions::draw_filled_rect({ position.x, position.y + slider_size.y + padding.y }, oneP * (*value - min), slider_size.y, colors::slider_progress.real());
		functions::draw_filled_rect({ position.x + oneP * (*value - min), position.y + slider_size.y + padding.y - 3.f }, 10.f, slider_size.y + 6.f, colors::slider_button.real());

		char value_test[64];
		sprintf(value_test, result.c_str(), *value);

		std::string value_string = value_test;
		std::wstring long_value(value_string.begin(), value_string.end());

		fvector2d value_position = { position.x + size.x - 26, position.y + 20 };
		functions::text_left(long_value.c_str(), value_position, 0.8f, { 1, 1, 1, 1 }, false);

		fvector2d text_position = { position.x - 2, position.y - 2 };
		functions::text_left(name, text_position, 0.8f, { 1, 1, 1, 1 }, false);

		defines::same_line = false;
		defines::last_element_pos = position;
		defines::last_element_size = size;

		if (defines::first_element_pos.x = 0.0f) defines::first_element_pos = position;
	}

	bool checkbox_enabled[256];
	void combobox(fstring name, fvector2d size, int* value, const char* arg, ...)
	{
		defines::elements_count++;

		fvector2d padding = { 10, 20 };
		fvector2d position = { defines::menu_position.x + padding.x + defines::offset_x, defines::menu_position.y + padding.y + defines::offset_y };

		if (defines::same_line) {
			position.x = defines::last_element_pos.x + defines::last_element_size.x + padding.x;
			position.y = defines::last_element_pos.y;
		}

		if (defines::pushY) {
			position.y = defines::pushYvalue;
			defines::pushY = false;
			defines::pushYvalue = 0.0f;
			defines::offset_y = position.y - defines::menu_position.y;
		}

		if (defines::pushX) {
			position.x = defines::pushXvalue;
			defines::pushX = false;
			defines::pushXvalue = 0.0f;
			defines::offset_x = position.x - defines::menu_position.x;
		}

		bool is_hovered = functions::is_mouse_in_zone(position, size);

		if (is_hovered || checkbox_enabled[defines::elements_count]) {
			functions::draw_filled_rect(position, size.x, size.y, colors::combobox_hovered.real());
			defines::hover_element = true;
		}
		else {
			functions::draw_filled_rect(position, size.x, size.y, colors::combobox_idle.real());
		}

		if (!defines::same_line)
			defines::offset_y += size.y + padding.y;

		fvector2d text_position = { position.x - 2, position.y - 18 };
		functions::text_left(name, text_position, 0.8f, { 1, 1, 1, 1 }, false);

		bool is_hovered_2 = false;
		fvector2d element_pos = position;
		int num = 0;

		if (checkbox_enabled[defines::elements_count]) {
			defines::current_element_size.x = element_pos.x - 5.0f;
			defines::current_element_size.y = element_pos.y - 5.0f;
		}

		va_list arguments;
		for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char*))
		{
			if (num == *value) {
				fvector2d _text_position = { position.x + size.x / 2, position.y + size.y / 2 };


				important_draw::text_center((char*)arg, _text_position, 0.8f, { 1, 1, 1, 1 }, false);
			}

			if (checkbox_enabled[defines::elements_count]) {
				element_pos.y += 25.f;

				is_hovered_2 = functions::is_mouse_in_zone(element_pos, { size.x, 25.f });

				if (is_hovered_2) {
					defines::hover_element = true;

					important_draw::draw_filled_rect(element_pos, size.x, 25.f, colors::combobox_hovered.real());

					if (controller* controller = blueprint::get_first_local_player_controller(globals::get_world_instance())) {
						if (input_handler::is_mouse_clicked(defines::elements_count, false)) {
							*value = num;
							checkbox_enabled[defines::elements_count] = false;
						}
					}
				}
				else {
					important_draw::draw_filled_rect(element_pos, size.x, 25.f, colors::combobox_idle.real());
				}

				important_draw::text_left((char*)arg, { element_pos.x + 5.f, element_pos.y + 5.f }, 0.8f, { 1, 1, 1, 1 }, false);
			}
			num++;
		}
		va_end(arguments);

		if (checkbox_enabled[defines::elements_count]) {
			defines::current_element_size.x = element_pos.x + 5.f;
			defines::current_element_size.y = element_pos.y + 5.f;
		}

		defines::same_line = false;
		defines::last_element_pos = position;
		defines::last_element_size = size;

		if (defines::first_element_pos.x = 0.0f) defines::first_element_pos = position;

		if (is_hovered && input_handler::is_mouse_clicked(defines::elements_count, false)) {
			checkbox_enabled[defines::elements_count] = !checkbox_enabled[defines::elements_count];
		}

		if (!is_hovered && !is_hovered_2 && input_handler::is_mouse_clicked(defines::elements_count, false)) {
			checkbox_enabled[defines::elements_count] = false;
		}

	}

	int active_picker = -1;
	flinearcolor saved_color;

	bool color_pixel(fvector2d pos, fvector2d size, flinearcolor* original, flinearcolor color)
	{
		important_draw::draw_filled_rect(fvector2d{ pos.x, pos.y }, size.x, size.y, color);

		if (original->red == color.red && original->green == color.green && original->blue == color.blue)
		{
			important_draw::draw_line({ pos.x, pos.y }, { pos.x + size.x - 1, pos.y }, 1.0f, { 0.0f, 0.0f, 0.0f, 1.0f });
			important_draw::draw_line({ pos.x, pos.y + size.y - 1 }, { pos.x + size.x - 1, pos.y + size.y - 1 }, 1.0f, { 0.0f, 0.0f, 0.0f, 1.0f });
			important_draw::draw_line({ pos.x, pos.y }, { pos.x, pos.y + size.y - 1 }, 1.0f, { 0.0f, 0.0f, 0.0f, 1.0f });
			important_draw::draw_line({ pos.x + size.x - 1, pos.y }, { pos.x + size.x - 1, pos.y + size.y - 1 }, 1.0f, { 0.0f, 0.0f, 0.0f, 1.0f });
		}

		if (functions::is_mouse_in_zone(pos, size))
			if (input_handler::is_mouse_clicked(defines::elements_count, false))
				*original = color;

		return true;
	}

	void color_picker(fstring name, flinearcolor* color) {
		defines::elements_count++;

		float size = 25;
		fvector2d padding = { 5, 10 };
		fvector2d position = { defines::menu_position.x + padding.x + defines::offset_x, defines::menu_position.y + padding.y + defines::offset_y };

		if (defines::same_line) {
			position.x = defines::last_element_pos.x + defines::last_element_size.x + padding.x;
			position.y = defines::last_element_pos.y;
		}

		if (defines::pushY) {
			position.y = defines::pushYvalue;
			defines::pushY = false;
			defines::pushYvalue = 0.0f;
			defines::offset_y = position.y - defines::menu_position.y;
		}

		if (defines::pushX) {
			position.x = defines::pushXvalue;
			defines::pushX = false;
			defines::pushXvalue = 0.0f;
			defines::offset_x = position.x - defines::menu_position.x;
		}

		bool is_hovered = functions::is_mouse_in_zone(position, { size, size });

		if (!defines::same_line)
			defines::offset_y += size + padding.y;

		if (active_picker == defines::elements_count) {
			defines::hover_element = true;

			fvector2d size_picker = { 250, 250 };
			bool is_hovered_picker = functions::is_mouse_in_zone(position, { size_picker.x, size_picker.y - 60 });

			important_draw::draw_filled_rect(position, size_picker.x, size_picker.y - 65, colors::colorpicker_background.real());

			fvector2d pixelSize = fvector2d{ size_picker.x / 12, size_picker.y / 12 };

			//0
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 0, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 174 / 255.f, 235 / 255.f, 253 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 0, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 136 / 255.f, 225 / 255.f, 251 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 0, position.y + pixelSize.y * 2 }, pixelSize, color, flinearcolor{ 108 / 255.f, 213 / 255.f, 250 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 0, position.y + pixelSize.y * 3 }, pixelSize, color, flinearcolor{ 89 / 255.f, 175 / 255.f, 213 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 0, position.y + pixelSize.y * 4 }, pixelSize, color, flinearcolor{ 76 / 255.f, 151 / 255.f, 177 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 0, position.y + pixelSize.y * 5 }, pixelSize, color, flinearcolor{ 60 / 255.f, 118 / 255.f, 140 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 0, position.y + pixelSize.y * 6 }, pixelSize, color, flinearcolor{ 43 / 255.f, 85 / 255.f, 100 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 0, position.y + pixelSize.y * 7 }, pixelSize, color, flinearcolor{ 32 / 255.f, 62 / 255.f, 74 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 0, position.y + pixelSize.y * 8 }, pixelSize, color, flinearcolor{ 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.0f });
			}
			//1
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 1, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 175 / 255.f, 205 / 255.f, 252 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 1, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 132 / 255.f, 179 / 255.f, 252 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 1, position.y + pixelSize.y * 2 }, pixelSize, color, flinearcolor{ 90 / 255.f, 152 / 255.f, 250 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 1, position.y + pixelSize.y * 3 }, pixelSize, color, flinearcolor{ 55 / 255.f, 120 / 255.f, 250 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 1, position.y + pixelSize.y * 4 }, pixelSize, color, flinearcolor{ 49 / 255.f, 105 / 255.f, 209 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 1, position.y + pixelSize.y * 5 }, pixelSize, color, flinearcolor{ 38 / 255.f, 83 / 255.f, 165 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 1, position.y + pixelSize.y * 6 }, pixelSize, color, flinearcolor{ 28 / 255.f, 61 / 255.f, 120 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 1, position.y + pixelSize.y * 7 }, pixelSize, color, flinearcolor{ 20 / 255.f, 43 / 255.f, 86 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 1, position.y + pixelSize.y * 8 }, pixelSize, color, flinearcolor{ 247 / 255.f, 247 / 255.f, 247 / 255.f, 1.0f });
			}
			//2
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 2, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 153 / 255.f, 139 / 255.f, 250 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 2, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 101 / 255.f, 79 / 255.f, 249 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 2, position.y + pixelSize.y * 2 }, pixelSize, color, flinearcolor{ 64 / 255.f, 50 / 255.f, 230 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 2, position.y + pixelSize.y * 3 }, pixelSize, color, flinearcolor{ 54 / 255.f, 38 / 255.f, 175 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 2, position.y + pixelSize.y * 4 }, pixelSize, color, flinearcolor{ 39 / 255.f, 31 / 255.f, 144 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 2, position.y + pixelSize.y * 5 }, pixelSize, color, flinearcolor{ 32 / 255.f, 25 / 255.f, 116 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 2, position.y + pixelSize.y * 6 }, pixelSize, color, flinearcolor{ 21 / 255.f, 18 / 255.f, 82 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 2, position.y + pixelSize.y * 7 }, pixelSize, color, flinearcolor{ 16 / 255.f, 13 / 255.f, 61 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 2, position.y + pixelSize.y * 8 }, pixelSize, color, flinearcolor{ 228 / 255.f, 228 / 255.f, 228 / 255.f, 1.0f });
			}
			//3
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 3, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 194 / 255.f, 144 / 255.f, 251 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 3, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 165 / 255.f, 87 / 255.f, 249 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 3, position.y + pixelSize.y * 2 }, pixelSize, color, flinearcolor{ 142 / 255.f, 57 / 255.f, 239 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 3, position.y + pixelSize.y * 3 }, pixelSize, color, flinearcolor{ 116 / 255.f, 45 / 255.f, 184 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 3, position.y + pixelSize.y * 4 }, pixelSize, color, flinearcolor{ 92 / 255.f, 37 / 255.f, 154 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 3, position.y + pixelSize.y * 5 }, pixelSize, color, flinearcolor{ 73 / 255.f, 29 / 255.f, 121 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 3, position.y + pixelSize.y * 6 }, pixelSize, color, flinearcolor{ 53 / 255.f, 21 / 255.f, 88 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 3, position.y + pixelSize.y * 7 }, pixelSize, color, flinearcolor{ 37 / 255.f, 15 / 255.f, 63 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 3, position.y + pixelSize.y * 8 }, pixelSize, color, flinearcolor{ 203 / 255.f, 203 / 255.f, 203 / 255.f, 1.0f });
			}
			//4
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 4, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 224 / 255.f, 162 / 255.f, 197 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 4, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 210 / 255.f, 112 / 255.f, 166 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 4, position.y + pixelSize.y * 2 }, pixelSize, color, flinearcolor{ 199 / 255.f, 62 / 255.f, 135 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 4, position.y + pixelSize.y * 3 }, pixelSize, color, flinearcolor{ 159 / 255.f, 49 / 255.f, 105 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 4, position.y + pixelSize.y * 4 }, pixelSize, color, flinearcolor{ 132 / 255.f, 41 / 255.f, 89 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 4, position.y + pixelSize.y * 5 }, pixelSize, color, flinearcolor{ 104 / 255.f, 32 / 255.f, 71 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 4, position.y + pixelSize.y * 6 }, pixelSize, color, flinearcolor{ 75 / 255.f, 24 / 255.f, 51 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 4, position.y + pixelSize.y * 7 }, pixelSize, color, flinearcolor{ 54 / 255.f, 14 / 255.f, 36 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 4, position.y + pixelSize.y * 8 }, pixelSize, color, flinearcolor{ 175 / 255.f, 175 / 255.f, 175 / 255.f, 1.0f });
			}
			//5
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 5, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 235 / 255.f, 175 / 255.f, 176 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 5, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 227 / 255.f, 133 / 255.f, 135 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 5, position.y + pixelSize.y * 2 }, pixelSize, color, flinearcolor{ 219 / 255.f, 87 / 255.f, 88 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 5, position.y + pixelSize.y * 3 }, pixelSize, color, flinearcolor{ 215 / 255.f, 50 / 255.f, 36 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 5, position.y + pixelSize.y * 4 }, pixelSize, color, flinearcolor{ 187 / 255.f, 25 / 255.f, 7 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 5, position.y + pixelSize.y * 5 }, pixelSize, color, flinearcolor{ 149 / 255.f, 20 / 255.f, 6 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 5, position.y + pixelSize.y * 6 }, pixelSize, color, flinearcolor{ 107 / 255.f, 14 / 255.f, 4 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 5, position.y + pixelSize.y * 7 }, pixelSize, color, flinearcolor{ 77 / 255.f, 9 / 255.f, 3 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 5, position.y + pixelSize.y * 8 }, pixelSize, color, flinearcolor{ 144 / 255.f, 144 / 255.f, 144 / 255.f, 1.0f });
			}
			//6
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 6, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 241 / 255.f, 187 / 255.f, 171 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 6, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 234 / 255.f, 151 / 255.f, 126 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 6, position.y + pixelSize.y * 2 }, pixelSize, color, flinearcolor{ 229 / 255.f, 115 / 255.f, 76 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 6, position.y + pixelSize.y * 3 }, pixelSize, color, flinearcolor{ 227 / 255.f, 82 / 255.f, 24 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 6, position.y + pixelSize.y * 4 }, pixelSize, color, flinearcolor{ 190 / 255.f, 61 / 255.f, 15 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 6, position.y + pixelSize.y * 5 }, pixelSize, color, flinearcolor{ 150 / 255.f, 48 / 255.f, 12 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 6, position.y + pixelSize.y * 6 }, pixelSize, color, flinearcolor{ 107 / 255.f, 34 / 255.f, 8 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 6, position.y + pixelSize.y * 7 }, pixelSize, color, flinearcolor{ 79 / 255.f, 25 / 255.f, 6 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 6, position.y + pixelSize.y * 8 }, pixelSize, color, flinearcolor{ 113 / 255.f, 113 / 255.f, 113 / 255.f, 1.0f });
			}
			//7
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 7, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 245 / 255.f, 207 / 255.f, 169 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 7, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 240 / 255.f, 183 / 255.f, 122 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 7, position.y + pixelSize.y * 2 }, pixelSize, color, flinearcolor{ 236 / 255.f, 159 / 255.f, 74 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 7, position.y + pixelSize.y * 3 }, pixelSize, color, flinearcolor{ 234 / 255.f, 146 / 255.f, 37 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 7, position.y + pixelSize.y * 4 }, pixelSize, color, flinearcolor{ 193 / 255.f, 111 / 255.f, 28 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 7, position.y + pixelSize.y * 5 }, pixelSize, color, flinearcolor{ 152 / 255.f, 89 / 255.f, 22 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 7, position.y + pixelSize.y * 6 }, pixelSize, color, flinearcolor{ 110 / 255.f, 64 / 255.f, 16 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 7, position.y + pixelSize.y * 7 }, pixelSize, color, flinearcolor{ 80 / 255.f, 47 / 255.f, 12 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 7, position.y + pixelSize.y * 8 }, pixelSize, color, flinearcolor{ 82 / 255.f, 82 / 255.f, 82 / 255.f, 1.0f });
			}
			//8
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 8, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 247 / 255.f, 218 / 255.f, 170 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 8, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 244 / 255.f, 200 / 255.f, 124 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 8, position.y + pixelSize.y * 2 }, pixelSize, color, flinearcolor{ 241 / 255.f, 182 / 255.f, 77 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 8, position.y + pixelSize.y * 3 }, pixelSize, color, flinearcolor{ 239 / 255.f, 174 / 255.f, 44 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 8, position.y + pixelSize.y * 4 }, pixelSize, color, flinearcolor{ 196 / 255.f, 137 / 255.f, 34 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 8, position.y + pixelSize.y * 5 }, pixelSize, color, flinearcolor{ 154 / 255.f, 108 / 255.f, 27 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 8, position.y + pixelSize.y * 6 }, pixelSize, color, flinearcolor{ 111 / 255.f, 77 / 255.f, 19 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 8, position.y + pixelSize.y * 7 }, pixelSize, color, flinearcolor{ 80 / 255.f, 56 / 255.f, 14 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 8, position.y + pixelSize.y * 8 }, pixelSize, color, flinearcolor{ 54 / 255.f, 54 / 255.f, 54 / 255.f, 1.0f });
			}
			//9
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 9, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 254 / 255.f, 243 / 255.f, 187 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 9, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 253 / 255.f, 237 / 255.f, 153 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 9, position.y + pixelSize.y * 2 }, pixelSize, color, flinearcolor{ 253 / 255.f, 231 / 255.f, 117 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 9, position.y + pixelSize.y * 3 }, pixelSize, color, flinearcolor{ 254 / 255.f, 232 / 255.f, 85 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 9, position.y + pixelSize.y * 4 }, pixelSize, color, flinearcolor{ 242 / 255.f, 212 / 255.f, 53 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 9, position.y + pixelSize.y * 5 }, pixelSize, color, flinearcolor{ 192 / 255.f, 169 / 255.f, 42 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 9, position.y + pixelSize.y * 6 }, pixelSize, color, flinearcolor{ 138 / 255.f, 120 / 255.f, 30 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 9, position.y + pixelSize.y * 7 }, pixelSize, color, flinearcolor{ 101 / 255.f, 87 / 255.f, 22 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 9, position.y + pixelSize.y * 8 }, pixelSize, color, flinearcolor{ 29 / 255.f, 29 / 255.f, 29 / 255.f, 1.0f });
			}
			//10
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 10, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 247 / 255.f, 243 / 255.f, 185 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 10, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 243 / 255.f, 239 / 255.f, 148 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 10, position.y + pixelSize.y * 2 }, pixelSize, color, flinearcolor{ 239 / 255.f, 232 / 255.f, 111 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 10, position.y + pixelSize.y * 3 }, pixelSize, color, flinearcolor{ 235 / 255.f, 229 / 255.f, 76 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 10, position.y + pixelSize.y * 4 }, pixelSize, color, flinearcolor{ 208 / 255.f, 200 / 255.f, 55 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 10, position.y + pixelSize.y * 5 }, pixelSize, color, flinearcolor{ 164 / 255.f, 157 / 255.f, 43 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 10, position.y + pixelSize.y * 6 }, pixelSize, color, flinearcolor{ 118 / 255.f, 114 / 255.f, 31 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 10, position.y + pixelSize.y * 7 }, pixelSize, color, flinearcolor{ 86 / 255.f, 82 / 255.f, 21 / 255.f, 1.0f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 10, position.y + pixelSize.y * 8 }, pixelSize, color, flinearcolor{ 9 / 255.f, 9 / 255.f, 9 / 255.f, 1.0f });
			}
			//11
			{
				color_pixel(fvector2d{ position.x + pixelSize.x * 11, position.y + pixelSize.y * 0 }, pixelSize, color, flinearcolor{ 2.093f, 0.019f, 20.0f, 5.9f });
				color_pixel(fvector2d{ position.x + pixelSize.x * 11, position.y + pixelSize.y * 1 }, pixelSize, color, flinearcolor{ 20.f, 9.77f, 0.617f, 2.153f });

			}

			if (!is_hovered_picker && input_handler::is_mouse_clicked(defines::elements_count, false)) {
				active_picker = -1;
			}
		}
		else
		{
			if (is_hovered) {
				functions::draw_filled_rect(position, size, size, colors::checkbox_hovered.real());
				defines::hover_element = true;
			}
			else {
				functions::draw_filled_rect(position, size, size, colors::checkbox_idle.real());
			}

			functions::draw_filled_rect({ position.x + 4, position.y + 4 }, size - 8, size - 8, *color);

			fvector2d text_position = { position.x + size + 5.0f, position.y + size / 2 };
			functions::text_left(name, text_position, 0.8f, { 1, 1, 1, 1 }, false);

			if (is_hovered && input_handler::is_mouse_clicked(defines::elements_count, false)) {
				saved_color = *color;
				active_picker = defines::elements_count;
			}
		}

		defines::same_line = false;
		defines::last_element_pos = position;

		if (defines::first_element_pos.x = 0.0f) defines::first_element_pos = position;
	}
}