#pragma once

#include <thread>
#include <algorithm>
namespace pinguu
{
	namespace menu
	{
		namespace defines {
			fvector2d position = { 100, 100 };
			fvector2d size = { 480.0f, 500.0f };

			fvector2d misc_button = { -260, 0 };

			int current_tab = 0;
			int current_sub_tab = 0;

			bool menu_open = false;
			bool menu_init = false;
			bool uninject = false;

			namespace items {
				bool old_cham_state = false;

				int hitbox_number = 0;
			}
		}

		void menu_tick() {

			if (!defines::menu_init)
				gui::defines::_canvas = globals::get_canvas();

			auto controller = blueprint::get_first_local_player_controller(globals::get_world_instance());
			if (!controller) return;


			if (controller->was_input_key_just_pressed(keys::insert)) {
				defines::menu_open = !defines::menu_open;

				if (defines::menu_open) {
					if (global::aimbot::config.iHitbox == 8) defines::items::hitbox_number = 0;
					else if (global::aimbot::config.iHitbox == 7) defines::items::hitbox_number = 1;
					else if (global::aimbot::config.iHitbox == 6) defines::items::hitbox_number = 2;

					uintptr_t AbilitiesAndBuffs = read<uintptr_t>((uintptr_t)controller->get_pawn() + offsets::actor::abilities_and_buffs);
					uintptr_t CachedAttributeSet = read<uintptr_t>((uintptr_t)AbilitiesAndBuffs + offsets::actor::cached_attribute_set);
					write<float>(CachedAttributeSet + offsets::cached_attribute_set::weapons_lowered + 0xC, 1.0);
				}
				else {
					pinguu::config::save_config();
					
					uintptr_t AbilitiesAndBuffs = read<uintptr_t>((uintptr_t)controller->get_pawn() + offsets::actor::abilities_and_buffs);
					uintptr_t CachedAttributeSet = read<uintptr_t>((uintptr_t)AbilitiesAndBuffs + offsets::actor::cached_attribute_set);
					write<float>(CachedAttributeSet + offsets::cached_attribute_set::weapons_lowered + 0xC, 0.0);
				}
				
			}

			if (gui::window(&defines::position, fvector2d{ 550.0f, 400.0f }, defines::menu_open))
			{
				//TABS
				static int tab = 0; //Changen
				if (gui::button_tab(Encrypt(L"AIMBOT").decrypt(), fvector2d{ 110, 25 }, 0, 0, tab == 0)) tab = 0;
				if (gui::button_tab(Encrypt(L"ESP / CHAMS").decrypt(), fvector2d{ 110, 25 }, 0, 0, tab == 1)) tab = 1;
				if (gui::button_tab(Encrypt(L"SKIN CHANGER").decrypt(), fvector2d{ 110, 25 }, 0, 0, tab == 2)) tab = 2;
				if (gui::button_tab(Encrypt(L"INSTA LOCK").decrypt(), fvector2d{ 110, 25 }, 0, 0, tab == 3)) tab = 3;
				if (gui::button_tab(Encrypt(L"MORE").decrypt(), fvector2d{ 110, 25 }, 0, 0, tab == 4)) tab = 4;
				gui::functions::next_column(130.0f);
				//

				//AIMBOT
				if (tab == 0) {
					gui::checkbox(Encrypt(L"ENABLE AIMBOT").decrypt(), 8, &global::aimbot::config.bEnabled);
					gui::checkbox(Encrypt(L"TARGET VISIBLE BONES ONLY").decrypt(), 8, &global::aimbot::config.bNearestVisibleLocation);

					gui::text(Encrypt(L"OR").decrypt());

					gui::checkbox(Encrypt(L"TARGET NEAREST BONES ONLY").decrypt(), 8, &global::aimbot::config.bNearestBone);

					if (global::aimbot::config.bNearestVisibleLocation && global::aimbot::config.bNearestBone) {
						global::aimbot::config.bNearestBone = false;
						global::aimbot::config.bNearestVisibleLocation = false;
					}

					gui::checkbox(Encrypt(L"CONSTANT MOVEMENT AS FAR AS POSSIBLE").decrypt(), 8, &global::aimbot::config.bConstant);
					gui::checkbox(Encrypt(L"AIMBOT IS RIGHT-CLICK FOR PISTOLS").decrypt(), 8, &global::aimbot::config.bAlternatePistoleKey);

					global::aimbot::config.fSpeed = std::clamp(global::aimbot::config.fSpeed, 0.f, 25.f);
					global::aimbot::config.fMaxFov = std::clamp(global::aimbot::config.fMaxFov, 0.f, 60.f);

					gui::functions::push_next_element_y(5, true);

					gui::slider_float(Encrypt(L"FOV").decrypt(), &global::aimbot::config.fMaxFov, 0, 60, EncryptGet("%.2fpx")), gui::functions::push_next_element_y(-5, true);
					gui::slider_float(Encrypt(L"SPEED").decrypt(), &global::aimbot::config.fSpeed, 0.1f, 25, EncryptGet("%.2f")), gui::functions::push_next_element_y(-5, true);
					gui::slider_float(Encrypt(L"RECOIL STRENGTH").decrypt(), &global::aimbot::config.fRecoilStrength, 0, 2, EncryptGet("%.2f"));

					gui::combobox(Encrypt(L"HITBOX").decrypt(), { 100, 25 }, &defines::items::hitbox_number, xorstr_("Head"), xorstr_("Neck"), xorstr_("Chest"), NULL); gui::functions::same_line();
					gui::combobox(Encrypt(L"AIMKEY").decrypt(), { 100, 25 }, &global::aimbot::config.iKey, xorstr_("LMouse"), xorstr_("RMouse"), xorstr_("Mouse 5"), xorstr_("Mouse 6"), xorstr_("LAlt"), xorstr_("LShift"), xorstr_("V"), NULL);

					if (defines::items::hitbox_number == 0) global::aimbot::config.iHitbox = 8;
					else if (defines::items::hitbox_number == 1) global::aimbot::config.iHitbox = 7;
					else if (defines::items::hitbox_number == 2) global::aimbot::config.iHitbox = 6;
				}
				//

				//ESP/CHAMS
				if (tab == 1) {
					gui::checkbox(Encrypt(L"ENABLE VISUALS").decrypt(), 8, &global::esp::config.bEnabled);
					gui::checkbox(Encrypt(L"SKELETON").decrypt(), 8, &global::esp::config.bSkeleton);
					gui::checkbox(Encrypt(L"SHOW NAMES").decrypt(), 8, &global::esp::config.bDrawNames);
					gui::checkbox(Encrypt(L"SHOW DORMANT").decrypt(), 8, &global::esp::config.bDrawDormant);
					gui::checkbox(Encrypt(L"SHOW HEALTH").decrypt(), 8, &global::esp::config.bDrawHealth);
					gui::checkbox(Encrypt(L"SHOW SPIKE").decrypt(), 8, &global::misc::config.bSpikeEsp);
					gui::checkbox(Encrypt(L"SHOW GUN-WIREFRAME").decrypt(), 8, &global::esp::config.bWireFrameGun);
					gui::checkbox(Encrypt(L"SHOW HAND-WIREFRAME").decrypt(), 8, &global::esp::config.bWireFrameHands);
					gui::checkbox(Encrypt(L"SHOW RECOIL").decrypt(), 8, &global::esp::config.bDrawRecoil);
					gui::checkbox(Encrypt(L"SHOW CHARACTER ICON").decrypt(), 8, &global::esp::config.bCharacterIcon);
					gui::checkbox(Encrypt(L"SHOW CHARACTER WEAPON").decrypt(), 8, &global::esp::config.bCharacterWeaponIcon);
					gui::checkbox(Encrypt(L"VISUALS ON HOTKEY").decrypt(), 8, &global::esp::config.bHotkeyVisuals); gui::functions::push_next_element_y(40, true);

					gui::slider_float(Encrypt(L"SKELETON THICKNESS").decrypt(), &global::esp::config.fSkeletonThickness, 0, 60, EncryptGet("%.2fpx")), gui::functions::push_next_element_y(20, true);

					gui::combobox(Encrypt(L"VISUALS MODE").decrypt(), { 100, 25 }, &global::esp::config.iEspType, xorstr_("Off"), xorstr_("2D Box"), xorstr_("3D Box"), NULL); gui::functions::same_line();
					gui::combobox(Encrypt(L"HOTKEY ESP").decrypt(), { 100, 25 }, &global::esp::config.iKey, xorstr_("LMouse"), xorstr_("RMouse"), xorstr_("Mouse 5"), xorstr_("Mouse 6"), xorstr_("LAlt"), xorstr_("LShift"), xorstr_("V"), NULL); gui::functions::same_line();
					gui::combobox(Encrypt(L"HOTKEY CHAMS").decrypt(), { 100, 25 }, &global::chams::config.iKey, xorstr_("LMouse"), xorstr_("RMouse"), xorstr_("Mouse 5"), xorstr_("Mouse 6"), xorstr_("LAlt"), xorstr_("LShift"), xorstr_("V"), NULL);

					gui::functions::push_next_element_x(339, false);
					gui::functions::push_next_element_y(25, false);

					gui::checkbox(Encrypt(L"ENABLE CHAMS").decrypt(), 8, &global::chams::config.bEnabled);
					gui::functions::push_next_element_x(-100, true);

					gui::checkbox(Encrypt(L"ALWAYS CHAMS").decrypt(), 8, &global::chams::config.bAlways);
					gui::functions::push_next_element_x(-100, true);

					gui::checkbox(Encrypt(L"CHAMS ON HOTKEY").decrypt(), 8, &global::chams::config.bHotkeyChams);
					gui::functions::push_next_element_x(-100, true);

					gui::checkbox(Encrypt(L"FRESNEL COLOR").decrypt(), 8, &global::fresnel::config.bEnabled);	
					gui::functions::push_next_element_x(-100, true);

					gui::text(Encrypt(L"ENEMY/ALLY COLOR").decrypt());
					gui::functions::push_next_element_x(-100, true);
					gui::functions::push_next_element_y(4, true);

					gui::text(Encrypt(L"CHAMS COLOR").decrypt());
				
					gui::functions::push_next_element_x(478, false);
					gui::functions::push_next_element_y(115, false);
					gui::color_picker(L"", &global::fresnel::config.flColor);

					gui::functions::push_next_element_x(508, false);
					gui::functions::push_next_element_y(115, false);
					gui::color_picker(L"", &global::fresnel::config.flAllyColor);

					gui::functions::push_next_element_x(448, false);
					gui::functions::push_next_element_y(145, false);
					gui::color_picker(L"", &global::chams::config.flColor);

					if (global::chams::config.bEnabled) {
						flinearcolor color = { global::chams::config.fR, global::chams::config.fG, global::chams::config.fB, global::chams::config.fA };
						if (!global::chams::config.bOwnColorMode) color = global::chams::config.flColor;
						outline_rendering::set_outline_colors_for_render(globals::get_world_instance(), color, { 1.f, 1.f, 0.f, 0.5f });
					}

					if (defines::items::old_cham_state != global::chams::config.bEnabled) {
						if (defines::items::old_cham_state == true && global::chams::config.bEnabled == false)
							pinguu::chams::disable_chams();

						defines::items::old_cham_state = global::chams::config.bEnabled;
					}
				}
				//
				if (tab == 2) {
					gui::checkbox(Encrypt(L"ENABLE SKIN CHANGER").decrypt(), 8, &global::skin_changer::config.bEnabled);

					if (global::skin_changer::config.bEnabled) {

						if (inventory_manager* manager = ares_instance::get_ares_client_game_instance(globals::get_world_instance())->get_inventory_manager()) {
							auto charm_instances = read<array<tmap<uobject*, uobject*>>>(std::uintptr_t(manager) + offsets::inventory_manager::charm_map);

							if (charm_instances.size()) {
								gui::slider_int(Encrypt(L"WEAPON CHARMS").decrypt(), &global::skin_changer::config.iGunBuddy, 0, charm_instances.size() - 1, EncryptGet("%d"));
								gui::functions::push_next_element_y(-20, true);
								gui::text(system::get_object_name(charm_instances[global::skin_changer::config.iGunBuddy].Key).c_str(), 0.8f);

								if (gui::button(Encrypt(L"APPLY WEAPON CHARM").decrypt(), { 150, 25 })) {
									if (auto pawn = controller->get_pawn()) {
										if (auto inventory = pawn->get_inventory()) {
											if (auto equippable = inventory->get_current_equippable()) {
												if (auto decrypted = system::decrypt_parent(equippable)) {
													auto a1 = read<uintptr_t>(decrypted + 0x318);
													a1 = read<uintptr_t>(a1 + 0xA0);

													auto old = read<int>(a1 + 0x80);
													write<int>(a1 + 0x80, 2);

													auto skin_data_asset = read<equippable_skin_data_asset*>(std::uintptr_t(equippable) + 0xdc8);
													auto skin_chroma_asset = read<uobject*>(std::uintptr_t(equippable) + 0xde8);

													content_library::clear_weapon_components(equippable);
													content_library::apply_skin(equippable, skin_data_asset, skin_chroma_asset, skin_data_asset->get_skin_levels().size(), charm_instances[global::skin_changer::config.iGunBuddy].Key, 1);

													config::save_config();

													write<int>(a1 + 0x80, old);
												}
											}
										}
									}
								}
							}
						}
					}
				}


				if (tab == 3) {
					gui::checkbox(Encrypt(L"INSTA LOCK").decrypt(), 8, &global::misc::config.bInstaLock);

					if (global::misc::config.bInstaLock) {
						auto agent = Encrypt(L"Agent");
						auto astra = Encrypt("Astra");
						auto breach = Encrypt("Breach");
						auto brimstone = Encrypt("Brimstone");
						auto chamber = Encrypt("Chamber");
						auto cypher = Encrypt("Cypher");
						auto fade = Encrypt("Fade");
						auto jett = Encrypt("Jett");
						auto kayo = Encrypt("Kayo");
						auto killjoy = Encrypt("Killjoy");
						auto neon = Encrypt("Neon");
						auto omen = Encrypt("Omen");
						auto phoenix = Encrypt("Phoenix");
						auto raze = Encrypt("Raze");
						auto reyna = Encrypt("Reyna");
						auto sage = Encrypt("Sage");
						auto skye = Encrypt("Skye");
						auto sova = Encrypt("Sova");
						auto viper = Encrypt("Viper");
						auto yoruo = Encrypt("Yoru");
						auto gekko = Encrypt("Gekko");

						gui::combobox(agent.decrypt(), { 100, 25 }, & global::misc::config.iInstaLockAgent,
							astra.decrypt(), breach.decrypt(), brimstone.decrypt(), chamber.decrypt(), cypher.decrypt(), fade.decrypt(), jett.decrypt(), kayo.decrypt(), killjoy.decrypt(), neon.decrypt(), omen.decrypt(),
							phoenix.decrypt(), raze.decrypt(), reyna.decrypt(), sage.decrypt(), skye.decrypt(), sova.decrypt(), viper.decrypt(), yoruo.decrypt(), gekko.decrypt(), NULL);

						if (global::misc::config.bRandomAgent)
							global::misc::config.bRandomAgent = false;
					}
					else {
						gui::checkbox(Encrypt(L"RANDOM AGENT").decrypt(), 8, &global::misc::config.bRandomAgent);

					}
				}

				if (tab == 4) {
					gui::checkbox(Encrypt(L"STANDALONE RECOIL").decrypt(), 8, &global::misc::config.bStandaloneRecoil);
					gui::checkbox(Encrypt(L"BUNNYHOP").decrypt(), 8, &global::misc::config.bBunnyhop);
					gui::checkbox(Encrypt(L"ANTI AFK").decrypt(), 8, &global::misc::config.bAntiAfk);
					gui::checkbox(Encrypt(L"RADAR HACK").decrypt(), 8, &global::misc::config.bRadarHack);

					if (gui::button(L"UNHOOK AND REMOVE CHEAT", { 250, 25 }))
					{
						if (actor* pawn = controller->get_pawn())
							pawn->enable_input(controller), write<bool>(std::uintptr_t(controller) + 0x5f0 >> 0, false);

						auto enc = Encrypt("[X] Uninjected - %s\n");
						printf(enc.decrypt(), __TIME__);
						enc.encrypt();

						menu::defines::uninject = true;
					}
				}
				

				gui::functions::push_next_element_x(460, false);
				gui::functions::push_next_element_y(370, false);
				gui::text(Encrypt(L"(c) BSELLING.XYZ").decrypt(), 0.75f, false, false);
			}

			gui::important_renderer();
		}
	}
}