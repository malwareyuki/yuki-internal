#pragma once

class BunnyHop {
public:
	void run() {
		if (!global::misc::config.bBunnyhop)
			return;

		if (actor* pawn = globals::get_local_pawn()) {
			if (pawn->can_jump()) {
				if (globals::get_player_controller()->is_input_key_down(pinguu::keys::space_bar)) {
					globals::get_player_controller()->simulate_input_key(pinguu::keys::space_bar), globals::get_player_controller()->simulate_input_key(pinguu::keys::space_bar, false);
				}
			}
		}
	}
};

class FresnelChanger {
public:
	void run() {
		if (!global::fresnel::config.bEnabled)
			return;

		if (auto local_pawn = globals::get_local_pawn()) {
			auto actors = globals::get_players();
			for (int i = 0; i < actors.size(); i++) {
				if (actor* actor = actors[i]) {
					if (actor != local_pawn) {
						if (global::fresnel::config.bEnabled) {


							if (auto fresnel_component = read<uobject*>(std::uintptr_t(local_pawn) + offsets::actor::fresnel_component)) {
								auto intensity = read<float>(std::uintptr_t(fresnel_component) + offsets::fresnel_component::fresnel_intensity);
								if (intensity != global::fresnel::config.fIntensity) {
									write<float>(std::uintptr_t(fresnel_component) + offsets::fresnel_component::fresnel_intensity, global::fresnel::config.fIntensity);
								}

							}

							array<material_instance_dynamic*> mesh3p_mids = read<array<material_instance_dynamic*>>(std::uintptr_t(actor) + offsets::actor::mesh_3p_mids);
							for (int a = 0; a < mesh3p_mids.size(); a++) {
								if (auto instance = mesh3p_mids[a]) {
									auto enc = Encrypt(L"Fresnel Enemy Tint");
									auto enc2 = Encrypt(L"Fresnel Ally Tint");
									instance->set_vector_parameter(string::string_to_name(enc.decrypt()), { 0, global::fresnel::config.flColor.red, global::fresnel::config.flColor.green, global::fresnel::config.flColor.blue }), enc.encrypt();
									instance->set_vector_parameter(string::string_to_name(enc2.decrypt()), { 0, global::fresnel::config.flAllyColor.red, global::fresnel::config.flAllyColor.green, global::fresnel::config.flAllyColor.blue }), enc2.encrypt();

									instance->set_scalar_parameter(string::string_to_name(L"Fresnel Enemy Offset"), 100);
									instance->set_scalar_parameter(string::string_to_name(L"Fresnel Ally Offset"), 100);

									/*instance->set_scalar_parameter(string::string_to_name(L"Distance"), 800.0);
									instance->set_scalar_parameter(string::string_to_name(L"Exp"), 300.0);
									instance->set_scalar_parameter(string::string_to_name(L"ExponentIn"), 555.0);
									instance->set_scalar_parameter(string::string_to_name(L"NoiseMult"), 2222.0);
									instance->set_scalar_parameter(string::string_to_name(L"Offset"), 222.18);
									instance->set_scalar_parameter(string::string_to_name(L"Opacity"), 1.0);
									instance->set_scalar_parameter(string::string_to_name(L"Outline Thickness Far"), 222);
									instance->set_scalar_parameter(string::string_to_name(L"Outline Thickness Near"), 222);
									instance->set_scalar_parameter(string::string_to_name(L"RefractionDepthBias"), 0.0);

									instance->set_vector_parameter(string::string_to_name(L"Emissive"), { 25.0, 18.062744, 4.29401, 0 });*/
								}
							}
						}
					}
				}
			}
		}
	}
};

class RadarHack {
public:
	void set_radar(bool status) {
		if (auto local_pawn = globals::get_local_pawn()) {
			auto actors = globals::get_players();

			for (int i = 0; i < actors.size(); i++) {
				if (auto actor = actors[i]; actor != local_pawn) {
					if (auto character_minimap = actor->get_character_minimap_component(); auto portrait_minimap = actor->get_portrait_minimap_component()) {

						if (actor->is_dormant() || !actor->get_mesh() || !actor->is_player_character())
							continue;

						character_minimap->set_observer_status(status);
						character_minimap->set_visible_status(status);

						portrait_minimap->set_observer_status(status);
						portrait_minimap->set_visible_status(status);
					}
				}
			}
		}
	}

	void run() {
		if (this->radarHackOld != global::misc::config.bRadarHack) {

			if (global::misc::config.bRadarHack == false) {
				set_radar(false);
			}

			this->radarHackOld = global::misc::config.bRadarHack;
		}

		if (!global::misc::config.bRadarHack)
			return;

		set_radar(true);
	}
private:
	bool radarHackOld = false;
};

class SpikeEsp {
public:
	void run() {
		if (!global::misc::config.bSpikeEsp)
			return;

		if (auto local_pawn = globals::get_local_pawn()) {
			auto func = Encrypt(L"PlantedBomb.PlantedBomb_C");
			array<actor*> planeted_bombs = gameplay_statics::get_all_actors_of_class<actor>(globals::get_world_instance(), uobject::find_object<uclass>(func.decrypt(), reinterpret_cast<uobject*>(-1)));
			func.encrypt();

			for (int i = 0; i < planeted_bombs.size(); i++) {
				if (auto bomb = planeted_bombs[i]) {

					float defuse_time = read<float>(std::uintptr_t(bomb) + offsets::bomb_offsets::defuse_progress);
					float bomb_time_remaining = read<float>(std::uintptr_t(bomb) + offsets::bomb_offsets::time_remaining_to_explode);

					flinearcolor color = { 1, 1, 1, 1 };

					fvector pos = globals::get_canvas()->project(bomb->get_location());
					if (!pos.z) continue;

					auto enc = Encrypt(L"Bomb (Remaining Time: %d Sec)");

					wchar_t buffer[64];
					swprintf(buffer, 64, enc.decrypt(), (int)bomb_time_remaining);
					enc.encrypt();


					if (bomb_time_remaining < 7)
						color = { 1, 0, 0, 1 };

					globals::get_canvas()->draw_text((::uobject*)NamesFont, buffer, { pos.x, pos.y }, { 1, 1 }, color, 1.0f, { 0, 0, 0, 1 }, { 1, 1 }, true, true, true, { 0,0,0,1 });

					fvector my_pos = globals::get_canvas()->project(local_pawn->get_location());

					globals::get_canvas()->draw_line({ my_pos.x, my_pos.y }, { pos.x, pos.y }, 1.f, color);
				}
			}
		}
	}
};

class AntiAfk {
public:
	void run() {
		if (!global::misc::config.bAntiAfk)
			return;

		if (auto pawn = globals::get_local_pawn()) {
			if (auto mesh = pawn->get_mesh()) {
				if (!this->timeOld) this->timeOld = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				int current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

				if (current_time > this->timeOld)
				{
					this->timeOld = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + 60000;

					globals::get_player_controller()->simulate_input_key(pinguu::keys::space_bar);
					globals::get_player_controller()->simulate_input_key(pinguu::keys::space_bar, false);
				}
			}
		}
	}
private:
	int timeOld = NULL;
};;

namespace pinguu
{
	namespace misc
	{
		void run(controller* controller)
		{
			static BunnyHop bunnyHop = BunnyHop();
			bunnyHop.run();

			static RadarHack radar_hack = RadarHack();
			radar_hack.run();

			static SpikeEsp spike_esp = SpikeEsp();
			spike_esp.run();

			static AntiAfk anti_afk = AntiAfk();
			anti_afk.run();

			static FresnelChanger fresnel_changer = FresnelChanger();
			fresnel_changer.run();
		}
	}
}
