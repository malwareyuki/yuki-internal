#pragma once

namespace pinguu
{
	namespace chams
	{
		typedef struct
		{
			DWORD R;
			DWORD G;
			DWORD B;
			DWORD A;
		}RGBA;

		namespace defines {
			flinearcolor current_rainbow_color = { 1, 0, 0, 1 };
			int old_time = NULL;
			RGBA espWave = { 255, 0, 0, 255 };
		}

		namespace functions {
			actor* get_local_pawn() {
				if (controller* controller = blueprint::get_first_local_player_controller(globals::get_world_instance()))
					if (actor* pawn = controller->get_pawn())
						return pawn;

				return nullptr;
			}
		}

		static void disable_chams() {
			if (actor* pawn = functions::get_local_pawn()) {
				auto actors = globals::get_players();


				for (int i = 0; i < actors.size(); i++) {
					if (actor* actor = actors[i]) {
						if (mesh* mesh = actor->get_mesh()) {
							outline_rendering::set_outline_colors_for_render(globals::get_world_instance(), { 0.086500, 0.730461, 0.508881, 0.501961 }, { 0.086500, 0.730461, 0.508881, 0.501961 });

							if (team_component::is_ally(pawn, actor)) mesh->set_outline_mode(outline_mode::outline);
							else mesh->set_outline_mode(outline_mode::none);
						}
					}
				}
			}
		}

		void rainbow_chams() {

			if (!defines::old_time) defines::old_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			int current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			if (current_time > defines::old_time)
			{
				defines::old_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + 10;

				if (defines::espWave.B != 500 && defines::espWave.G == 0) {
					defines::espWave.B++;
				}

				if (defines::espWave.B == 500 && defines::espWave.R != 0) {
					defines::espWave.R--;
				}

				if (defines::espWave.B == 500 && defines::espWave.G != 500 && defines::espWave.R == 0) {
					defines::espWave.G++;
				}

				if (defines::espWave.G == 500 && defines::espWave.B != 0) {
					defines::espWave.B--;
				}

				if (defines::espWave.B == 0 && defines::espWave.R != 500) {
					defines::espWave.R++;
				}

				if (defines::espWave.R == 500 && defines::espWave.G != 0) {
					defines::espWave.G--;
				}

				defines::current_rainbow_color = flinearcolor(float(defines::espWave.R / 255.0), float(defines::espWave.G / 255.0), float(defines::espWave.B / 255.0), float(defines::espWave.A / 255.0));
				outline_rendering::set_outline_colors_for_render(globals::get_world_instance(), defines::current_rainbow_color, { 1.f, 1.f, 0.f, 0.5f });
			}
		}

		void run(controller* controller)
		{
			if (global::chams::config.bHotkeyChams) {
				if (controller->is_input_key_down(pinguu::keys::convert_key(global::chams::config.iKey))) {
					global::chams::config.bEnabled = true;
				}
				else {
					if (global::chams::config.bEnabled == true) {
						global::chams::config.bEnabled = false;
						disable_chams();
					}
				}
			}

			if (global::chams::config.bEnabled)
			{
				if (actor* pawn = controller->get_pawn())
				{
					auto actors = globals::get_players();

					if (global::chams::config.bRainbowChams)
					{
						rainbow_chams();
					}
					else
					{
						flinearcolor color = { global::chams::config.fR, global::chams::config.fG, global::chams::config.fB, global::chams::config.fA };
						if (!global::chams::config.bOwnColorMode) color = global::chams::config.flColor;
					}

					for (int i = 0; i < actors.size(); i++)
					{
						if (actor* actor = actors[i])
						{
							if (actor != pawn)
							{
								if (!actor->is_alive()) continue;

								if (mesh* mesh = actor->get_mesh())
								{
									if (team_component::is_ally(controller->get_pawn(), actor))
									{
										// Remove Team Outline
										mesh->set_outline_mode(outline_mode::none);

									}
									else
									{
										// Set Enemy Outline
										mesh->set_outline_mode(global::chams::config.bAlways ? outline_mode::always_outline : outline_mode::outline);
									}
								}
							}
						}

					}
				}
			}
		}
	}
}