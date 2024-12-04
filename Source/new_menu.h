namespace new_menu {
	namespace defines {
		float esp_color[4] = { 255.0f, 0.0f, 0.0f, 1.0f };
		float colSkelColor[4] = { 1, 0, 0, 1 };

		namespace items {
			int hitbox_number = 0;
		}
	}

	void aimbot_menu() {
		Gui::startSubmenu(EncryptGet("General"), Side::Left);

		Gui::addCheckbox(EncryptGet("Enable Aimbot"), &global::aimbot::config.bEnabled);

		auto hitbox_enc = Encrypt("Head\0\rNeck\0\rChest\0\0");
		Gui::addDropdown(EncryptGet("Hitbox"), hitbox_enc.decrypt(), &defines::items::hitbox_number), hitbox_enc.encrypt();

		auto key_enc = Encrypt("LMouse\0\rRMouse\0\rMouse 5\0\rMouse 6\0\rLAlt\0\rLShift\0\rV\0\0");
		Gui::addDropdown(EncryptGet("Aimkey"), key_enc.decrypt(), &global::aimbot::config.iKey), key_enc.encrypt();

		Gui::addCheckbox(EncryptGet("Nearest Bone"), &global::aimbot::config.bNearestBone);
		Gui::addCheckbox(EncryptGet("Nearest Visible Location"), &global::aimbot::config.bNearestVisibleLocation);
		Gui::addCheckbox(EncryptGet("Constant Aimbot"), &global::aimbot::config.bConstant);
		Gui::addCheckbox(EncryptGet("Alternate Pistol Key"), &global::aimbot::config.bAlternatePistoleKey);
		Gui::addCheckbox(EncryptGet("Auto Shot"), &global::aimbot::config.bAutoShot);

		Gui::endSubmenu();
#
		Gui::startSubmenu(EncryptGet("Slider"), Side::Right);

		Gui::addSliderFloat(EncryptGet("Max FOV"), &global::aimbot::config.fMaxFov, 0.1f, 10.0f);
		Gui::addSliderFloat(EncryptGet("Speed"), &global::aimbot::config.fSpeed, 0.1f, 25.0f);
		Gui::addSliderFloat(EncryptGet("Recoil Strength"), &global::aimbot::config.fRecoilStrength, 0.0f, 2.0f);

		Gui::endSubmenu();

		if (defines::items::hitbox_number == 0) global::aimbot::config.iHitbox = 8;
		else if (defines::items::hitbox_number == 1) global::aimbot::config.iHitbox = 7;
		else if (defines::items::hitbox_number == 2) global::aimbot::config.iHitbox = 6;
	}

	void triggerbot_menu() {
		Gui::startSubmenu(EncryptGet("Triggerbot"), Side::Middle);

		Gui::addCheckbox(EncryptGet("Enable Triggerbot"), &global::triggerbot::config.bEnabled);
		Gui::addCheckbox(EncryptGet("Shot Once"), &global::triggerbot::config.bShotOnce);
		Gui::addSliderInt(EncryptGet("Delay"), &global::triggerbot::config.iDelay, 0, 500);

		auto key_enc = Encrypt("LMouse\0\rRMouse\0\rMouse 5\0\rMouse 6\0\rLAlt\0\rLShift\0\rV\0\0");
		Gui::addDropdown(EncryptGet("Key"), key_enc.decrypt(), &global::triggerbot::config.iKey), key_enc.encrypt();

		Gui::endSubmenu();
	}

	void misc_menu() {
		Gui::startSubmenu(EncryptGet("General"), Side::Left);

		Gui::addCheckbox(EncryptGet("Standalone Recoil"), &global::misc::config.bStandaloneRecoil);
		Gui::addCheckbox(EncryptGet("Bunnyhop"), &global::misc::config.bBunnyhop);
		Gui::addCheckbox(EncryptGet("Debug"), &global::misc::config.bDebug);

		Gui::addCheckbox(EncryptGet("Radar Hack"), &global::misc::config.bRadarHack);
		Gui::addCheckbox(EncryptGet("Spike ESP"), &global::misc::config.bSpikeEsp);
		Gui::addCheckbox(EncryptGet("Anti AFK"), &global::misc::config.bAntiAfk);

		Gui::endSubmenu();

		Gui::startSubmenu(EncryptGet(" "), Side::Right);

		Gui::addCheckbox(EncryptGet("Insta Lock"), &global::misc::config.bInstaLock);

		if (global::misc::config.bInstaLock) {
			auto agent_enc = Encrypt("Agent\0\rAstra\0\rBreach\0\rBrimstone\0\rChamber\0\rCypher\0\rFade\0\rJett\0\rKayo\0\rKilljoy\0\rNeon\0\rOmen\0\rPhoenix\0\rRaze\0\rReyna\0\rSage\0\rSkye\0\rSova\0\rViper\0\rYoru\0\rGekko\0\0");
			Gui::addDropdown(EncryptGet("Agent"), agent_enc.decrypt(), &global::misc::config.iInstaLockAgent), agent_enc.encrypt();

			if (global::misc::config.bRandomAgent)
				global::misc::config.bRandomAgent = false;
		}
		else {
			Gui::addCheckbox(EncryptGet("Random Agent"), &global::misc::config.bRandomAgent);
		}

		Gui::endSubmenu();
	}

	void esp_menu() {
		Gui::startSubmenu(EncryptGet("ESP"), Side::Left);

		Gui::addCheckbox(EncryptGet("Enable ESP"), &global::esp::config.bEnabled);
		Gui::addCheckbox(EncryptGet("Skeleton"), &global::esp::config.bSkeleton);
		Gui::addCheckbox(EncryptGet("Draw Skeleton Head"), &global::esp::config.bDrawSkeletonHead);
		Gui::addColorPicker(EncryptGet("SkelHeadColor"), defines::colSkelColor, false);

		if (global::esp::config.colSkelHeadR != (defines::colSkelColor[0] / 255.0f)) {
			global::esp::config.colSkelHeadR = defines::colSkelColor[0] / 255.0f;
		}

		if (global::esp::config.colSkelHeadG != (defines::colSkelColor[1] / 255.0f)) {
			global::esp::config.colSkelHeadG = defines::colSkelColor[1] / 255.0f;
		}

		if (global::esp::config.colSkelHeadB != (defines::colSkelColor[2] / 255.0f)) {
			global::esp::config.colSkelHeadB = defines::colSkelColor[2] / 255.0f;
		}

		Gui::addCheckbox(EncryptGet("Hotkey Visuals"), &global::esp::config.bHotkeyVisuals);
		Gui::addCheckbox(EncryptGet("Draw Names"), &global::esp::config.bDrawNames);
		Gui::addCheckbox(EncryptGet("Draw Health"), &global::esp::config.bDrawHealth);
		Gui::addCheckbox(EncryptGet("Draw Dormant"), &global::esp::config.bDrawDormant);

		Gui::endSubmenu();

		Gui::startSubmenu(EncryptGet("General"), Side::Right);

		Gui::addCheckbox(EncryptGet("WireFrame Gun"), &global::esp::config.bWireFrameGun);
		Gui::addCheckbox(EncryptGet("WireFrame Hands"), &global::esp::config.bWireFrameHands);
		Gui::addCheckbox(EncryptGet("Draw Recoil"), &global::esp::config.bDrawRecoil);
		Gui::addCheckbox(EncryptGet("Character Icon"), &global::esp::config.bCharacterIcon);
		Gui::addCheckbox(EncryptGet("Character Weapon Icon"), &global::esp::config.bCharacterWeaponIcon);

		Gui::addSliderFloat(EncryptGet("Thickness"), &global::esp::config.fSkeletonThickness, 0.1f, 5.0f);

		auto visual_enc = Encrypt("Off\0\r2D Box\0\r3D Box\0\0");
		Gui::addDropdown(EncryptGet("Visual Mode"), visual_enc.decrypt(), &global::esp::config.iEspType), visual_enc.encrypt();

		auto key_enc = Encrypt("LMouse\0\rRMouse\0\rMouse 5\0\rMouse 6\0\rLAlt\0\rLShift\0\rV\0\0");
		Gui::addDropdown(EncryptGet("Key"), key_enc.decrypt(), &global::esp::config.iKey), key_enc.encrypt();

		Gui::endSubmenu();
	}

	void chams_menu() {
		Gui::startSubmenu(EncryptGet("Chams"), Side::Left);

		Gui::addCheckbox(EncryptGet("Enable Chams"), &global::chams::config.bEnabled);
		Gui::addCheckbox(EncryptGet("Always Chams"), &global::chams::config.bAlways);
		Gui::addCheckbox(EncryptGet("Hide Team Outlines"), &global::chams::config.bHideTeamOutlines);
		Gui::addCheckbox(EncryptGet("Hotkey Chams"), &global::chams::config.bHotkeyChams);
		Gui::addCheckbox(EncryptGet("Rainbow Chams"), &global::chams::config.bRainbowChams);
		Gui::addCheckbox(EncryptGet("Own Color Mode"), &global::chams::config.bOwnColorMode);

		Gui::addSliderInt(EncryptGet("Key"), &global::chams::config.iKey, 0, 5);
		Gui::addSliderInt(EncryptGet("Outline Mode"), &global::chams::config.iOutlineMode, 0, 5);

		Gui::addSliderFloat(EncryptGet("Red"), &global::chams::config.fR, 0.0f, 255.0f);
		Gui::addSliderFloat(EncryptGet("Green"), &global::chams::config.fG, 0.0f, 255.0f);
		Gui::addSliderFloat(EncryptGet("Blue"), &global::chams::config.fB, 0.0f, 255.0f);
		Gui::addSliderFloat(EncryptGet("Alpha"), &global::chams::config.fA, 0.0f, 255.0f);

		Gui::endSubmenu();
	}

	void fresnel_menu() {
		Gui::startSubmenu(EncryptGet("Fresnel"), Side::Left);

		Gui::addCheckbox(EncryptGet("Enable Fresnel"), &global::fresnel::config.bEnabled);
		Gui::addColorPicker(EncryptGet("Fresnel Color"), defines::esp_color), global::chams::config.flColor = flinearcolor(defines::esp_color[0] / 255.0f, defines::esp_color[1] / 255.0f, defines::esp_color[2] / 255.0f, defines::esp_color[3] / 255.0f); // Wenn überhaupt 255 kopmmt 


		Gui::addSliderFloat(EncryptGet("Intensity"), &global::fresnel::config.fIntensity, 0.0f, 1.0f);


		//Gui::addSliderFloat(EncryptGet("Red"), &global::fresnel::config.flColor.red, 0.0f, 255.0f);
		//Gui::addSliderFloat(EncryptGet("Green"), &global::fresnel::config.flColor.green, 0.0f, 255.0f);
		//Gui::addSliderFloat(EncryptGet("Blue"), &global::fresnel::config.flColor.blue, 0.0f, 255.0f);
		//Gui::addSliderFloat(EncryptGet("Alpha"), &global::fresnel::config.flColor.alpha, 0.0f, 255.0f);

		Gui::addSliderFloat(EncryptGet("Ally Red"), &global::fresnel::config.flAllyColor.red, 0.0f, 255.0f);
		Gui::addSliderFloat(EncryptGet("Ally Green"), &global::fresnel::config.flAllyColor.green, 0.0f, 255.0f);
		Gui::addSliderFloat(EncryptGet("Ally Blue"), &global::fresnel::config.flAllyColor.blue, 0.0f, 255.0f);
		Gui::addSliderFloat(EncryptGet("Ally Alpha"), &global::fresnel::config.flAllyColor.alpha, 0.0f, 255.0f);

		Gui::endSubmenu();
	}

	void skin_changer_menu() {
		Gui::startSubmenu(EncryptGet("Skin Changer"), Side::Left);

		Gui::addCheckbox(EncryptGet("Enable Skin Changer"), &global::skin_changer::config.bEnabled);

		if (global::skin_changer::config.bEnabled) {
			if (auto controller = blueprint::get_first_local_player_controller(globals::get_world_instance())) {
				if (inventory_manager* manager = ares_instance::get_ares_client_game_instance(globals::get_world_instance())->get_inventory_manager()) {
					auto charm_instances = read<array<tmap<uobject*, uobject*>>>(std::uintptr_t(manager) + offsets::inventory_manager::charm_map);

					if (charm_instances.size()) {
						Gui::addSliderInt(EncryptGet("Weapon Charms"), &global::skin_changer::config.iGunBuddy, 0, charm_instances.size() - 1);

						std::wstring weaponName = system::get_object_name(charm_instances[global::skin_changer::config.iGunBuddy].Key).c_str();
						std::string weaponNameStr(weaponName.begin(), weaponName.end());

						ImGui::Text(weaponNameStr.c_str());

						if (ImGui::Button(EncryptGet("Apply Weapon Charm").c_str(), { 150, 25 })) {
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

											pinguu::config::save_config();

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


		Gui::endSubmenu();
	}

	void draw_menu() {

		if (auto controller = blueprint::get_first_local_player_controller(globals::get_world_instance())) {

			static bool visible = false;
			if (IFH(GetAsyncKeyState)(VK_DELETE) & 1) {
				visible = !visible;

				if (visible) {
					if (global::aimbot::config.iHitbox == 8) defines::items::hitbox_number = 0;
					else if (global::aimbot::config.iHitbox == 7) defines::items::hitbox_number = 1;
					else if (global::aimbot::config.iHitbox == 6) defines::items::hitbox_number = 2;

					uintptr_t AbilitiesAndBuffs = read<uintptr_t>((uintptr_t)controller->get_pawn() + offsets::actor::abilities_and_buffs);
					uintptr_t CachedAttributeSet = read<uintptr_t>((uintptr_t)AbilitiesAndBuffs + offsets::actor::cached_attribute_set);
					write<float>(CachedAttributeSet + offsets::cached_attribute_set::weapons_lowered + 0xC, 1.0);

					defines::esp_color[0] = global::chams::config.flColor.red;
					defines::esp_color[1] = global::chams::config.flColor.green;
					defines::esp_color[2] = global::chams::config.flColor.blue;
					defines::esp_color[3] = global::chams::config.flColor.alpha;

					if (actor* pawn = controller->get_pawn()) {
						pawn->disable_input(controller);
						write<bool>(std::uintptr_t(controller) + offsets::controller::show_mouse_cursor >> 0, true);
					}
				}
				else {
					pinguu::config::save_config();

					uintptr_t AbilitiesAndBuffs = read<uintptr_t>((uintptr_t)controller->get_pawn() + offsets::actor::abilities_and_buffs);
					uintptr_t CachedAttributeSet = read<uintptr_t>((uintptr_t)AbilitiesAndBuffs + offsets::actor::cached_attribute_set);
					write<float>(CachedAttributeSet + offsets::cached_attribute_set::weapons_lowered + 0xC, 0.0);

					if (actor* pawn = controller->get_pawn()) {
						pawn->enable_input(controller);
						write<bool>(std::uintptr_t(controller) + offsets::controller::show_mouse_cursor >> 0, false);
					}

				}
			}

			if (visible)
			{
				if (gl_alpha < 1.f)
					gl_alpha += 0.01f * ImGui::GetIO().DeltaTime * 1000.f;
				else
					gl_alpha = 1.f;

				Gui::menu_visible = true;
			}
			else
			{
				if (gl_alpha > 0.f)
					gl_alpha -= 0.01f * ImGui::GetIO().DeltaTime * 1000.f;
				else
					gl_alpha = 0.f;

				Gui::menu_visible = false;
			}
		}

		Gui::addSidebarObject(EncryptGet("AIMBOT"), EncryptGet("Aimbot"), EncryptGet("Allows you to automatically aim at enemies"), ICON_FA_CROSSHAIRS, aimbot_menu);
		Gui::addSidebarObject(EncryptGet("TRIGGER"), EncryptGet("Triggerbot"), EncryptGet("Allows you to automatically shoot at enemies"), ICON_FA_BULLSEYE, triggerbot_menu);
		Gui::addSidebarObject(EncryptGet("MISC"), EncryptGet("Other Settings"), EncryptGet("Miscellaneous options"), ICON_FA_COGS, misc_menu);
		Gui::addSidebarObject(EncryptGet("ESP"), EncryptGet("ESP"), EncryptGet("Visual information about enemies"), ICON_FA_EYE, esp_menu);
		Gui::addSidebarObject(EncryptGet("CHAMS"), EncryptGet("Chams"), EncryptGet("Customize player appearance"), ICON_FA_PALETTE, chams_menu);
		Gui::addSidebarObject(EncryptGet("FRESNEL"), EncryptGet("Fresnel"), EncryptGet("Customize Fresnel effect"), ICON_FA_LIGHTBULB, fresnel_menu);
		Gui::addSidebarObject(EncryptGet("SKINS"), EncryptGet("Skin Changer"), EncryptGet("Customize player skins"), ICON_FA_PAINT_BRUSH, skin_changer_menu);
		Gui::renderGUI();
	}
}