#pragma once

#define IM_PI                   3.14159265358979323846f
#define DEG2RAD( x ) ( (float)(x) * (float)(IM_PI / 180.f) )
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / IM_PI) )

namespace pinguu
{
	namespace esp
	{
		static float distance(fvector a, fvector b) {
			return float(sqrtf(powf(a.x - b.x, 2.0) + powf(a.y - b.y, 2.0) + powf(a.z - b.z, 2.0)));
		}

		void draw_rect(float x, float y, int w, int h, flinearcolor color) {
			globals::get_canvas()->draw_line({ x, y }, { x + w, y }, 1.0f, color);
			globals::get_canvas()->draw_line({ x, y }, { x, y + h }, 1.0f, color);
			globals::get_canvas()->draw_line({ x + w, y }, { x + w, y + h }, 1.0f, color);
			globals::get_canvas()->draw_line({ x, y + h }, { x + w + 1, y + h }, 1.0f, color);
		}

		void draw_filled_rec(float x, float y, float w, float h, const flinearcolor& color) {
			globals::get_canvas()->draw_line({ x, y }, { x, y + h }, 2.0f, color);
		}


		void draw_recoil(actor* pawn, controller* controller) {
			if (!global::esp::config.bDrawRecoil) return;

			if (camera* camera = controller->get_camera()) {
				if (inventory* inventory = pawn->get_inventory()) {
					if (equippable* equippable = inventory->get_current_equippable()) {
						if (equippable->should_present_as_usable()) return;

						float screen_width = globals::get_canvas()->getSizeX();
						float screen_height = globals::get_canvas()->getSizeY();

						frotator control_rotation = controller->get_control_rotation();
						frotator camera_rotation = camera->get_rotation();

						fvector punch_angle = (camera_rotation - control_rotation);
						punch_angle.clamp();

						float center_x = screen_width / 2;
						float center_y = screen_height / 2;

						int d_x = screen_width / 90;
						int d_y = screen_height / 90;

						center_x += (d_x * (punch_angle.y));
						center_y -= (d_y * (punch_angle.x));

						globals::get_canvas()->draw_circle(center_x, center_y, 5, 60, { 1, 0, 0, 1 });
					}
				}
			}
		}

		void draw_esp_box(actor* actor, int boxType) {
			if (auto mesh = actor->get_mesh()) {
				const auto head = mesh->bone(8);
				const auto root = mesh->bone(0);

				const auto head_project = globals::get_canvas()->project(head);
				const auto root_project = globals::get_canvas()->project(root);

				if (!head_project.z || !root_project.z)
					return;

				flinearcolor color = { 1,0,0,1 }; // Default ESP Color

				auto enc_1 = Encrypt(L"decoy");
				auto enc_2 = Encrypt(L"TrainingBot");
				auto enc_3 = Encrypt(L"targetpracticedummy");

				if (string::contains(system::get_object_name(actor).c_str(), enc_1.decrypt())) { // If it's decoy
					color = { 0.98,0.011,0.98,1 };
				}
				else if (string::contains(system::get_object_name(actor).c_str(), enc_2.decrypt()) || string::contains(system::get_object_name(actor).c_str(), enc_3.decrypt())) { // if it's train bot
					color = { 0,1,0.74,1 };
				}
				else { // if it's a player

					// Color by health
				/*	if (actor->get_shield() > 0) color = { 0, 0, (float)((actor->get_shield() + 50) / 100), 1 };
					else if (actor->get_health() >= 75) color = { 0, 1, 0, 1 };
					else color = { 1, (float)(actor->get_health() / 100), 0, 1 };*/

					// Normal Color
					color = { 1, 0, 0, 1 };
				}

				if (actor->is_dormant()) color = { 0.70, 0.70, 0.70, 0.65 }; // Farbe wenn der Spieler Dormant ist

				enc_1.encrypt();
				enc_2.encrypt();
				enc_3.encrypt();

				float
					box_height = abs((head_project.y) - (root_project.y + 10)),
					box_width = box_height / 2.5f,
					box_thickness = 1.0f;

				if (boxType == 1) {
					draw_rect(root_project.x - box_width / 2, head_project.y - 5, box_width, box_height, color);
				}
				else if (boxType == 2) {
					fvector top[] = {
						globals::get_canvas()->project({root.x + 40.f, root.y + 40.f, head.z + 20.f}),
						globals::get_canvas()->project({root.x - 40.f, root.y - 40.f, head.z + 20.f}),
						globals::get_canvas()->project({root.x + 40.f, root.y - 40.f, head.z + 20.f}),
						globals::get_canvas()->project({root.x - 40.f, root.y + 40.f, head.z + 20.f})
					};

					fvector bottom[] = {
						globals::get_canvas()->project({root.x + 40.f, root.y + 40.f, root.z}),
						globals::get_canvas()->project({root.x - 40.f, root.y - 40.f, root.z}),
						globals::get_canvas()->project({root.x + 40.f, root.y - 40.f, root.z}),
						globals::get_canvas()->project({root.x - 40.f, root.y + 40.f, root.z})
					};

					if (!top[0].z || !top[1].z || !top[2].z || !top[3].z)
						return;

					if (!bottom[0].z || !bottom[1].z || !bottom[2].z || !bottom[3].z)
						return;

					globals::get_canvas()->draw_line({ bottom[0].x, bottom[0].y }, { bottom[2].x, bottom[2].y }, 1.f, color);
					globals::get_canvas()->draw_line({ bottom[2].x, bottom[2].y }, { bottom[1].x, bottom[1].y }, 1.f, color);
					globals::get_canvas()->draw_line({ bottom[1].x, bottom[1].y }, { bottom[3].x, bottom[3].y }, 1.f, color);
					globals::get_canvas()->draw_line({ bottom[3].x, bottom[3].y }, { bottom[0].x, bottom[0].y }, 1.f, color);

					globals::get_canvas()->draw_line({ top[0].x, top[0].y }, { top[2].x, top[2].y }, 1.f, color);
					globals::get_canvas()->draw_line({ top[2].x, top[2].y }, { top[1].x, top[1].y }, 1.f, color);
					globals::get_canvas()->draw_line({ top[1].x, top[1].y }, { top[3].x, top[3].y }, 1.f, color);
					globals::get_canvas()->draw_line({ top[3].x, top[3].y }, { top[0].x, top[0].y }, 1.f, color);

					globals::get_canvas()->draw_line({ bottom[0].x, bottom[0].y }, { top[0].x, top[0].y }, 1.f, color);
					globals::get_canvas()->draw_line({ bottom[1].x, bottom[1].y }, { top[1].x, top[1].y }, 1.f, color);
					globals::get_canvas()->draw_line({ bottom[2].x, bottom[2].y }, { top[2].x, top[2].y }, 1.f, color);
					globals::get_canvas()->draw_line({ bottom[3].x, bottom[3].y }, { top[3].x, top[3].y }, 1.f, color);
				}

				// Health
				if (global::esp::config.bDrawHealth) {
					draw_filled_rec(head_project.x - ((box_width + 5) / 2), (head_project.y - 5) + (box_height * (100 - actor->get_health()) / 100),
						2, box_height - (box_height * (100 - actor->get_health()) / 100), { (255 - actor->get_health()) / 255, (55 + actor->get_health() * 2) / 255, 75 / 255, 1 });
				}

				// Icon (Character)
				if (global::esp::config.bCharacterIcon) {
					if (auto portraint_minimap = read<uobject*>(std::uintptr_t(actor) + offsets::radar::portrait_minimap)) {
						if (auto character_icon = read<utexture*>(std::uintptr_t(portraint_minimap) + offsets::portrait_minimap::character_icon)) {
							globals::get_canvas()->draw_texture(character_icon, { head_project.x - 11.5f, head_project.y - 35.3f }, { 24, 24 }, { 0, 0 }, { 1, 1 }, { 1,1,1,1 }, 2, 0, { 0.f , 0.f });
						}
					}
				}

				// Icon (Weapon)
				if (global::esp::config.bCharacterWeaponIcon) {
					if (auto inventory = actor->get_inventory()) {
						if (auto equippable = inventory->get_current_equippable()) {
							if (auto icon = equippable->get_icon()) {
								globals::get_canvas()->draw_texture(icon, { head_project.x - 11.5f, root_project.y + 12.8f }, { 32, 16 }, { 0, 0 }, { 1, 1 }, { 1,1,1,1 }, 2, 0, { 0.f , 0.f });
							}
						}
					}
				}
			}
		}

		void draw_names(actor* actor)
		{
			fstring player_name = system::get_object_name(actor);

			if (!global::misc::config.bDebug)
			{
				player_state* player_state = actor->get_player_state();
				if (!player_state)
					return;

				player_name = player_state->get_player_name();
			}

			fvector root_bone = actor->get_mesh()->bone(8);
			fvector root_out = globals::get_canvas()->project(root_bone);

			if (!root_out.z)
				return;

			const auto font = (::uobject*)NamesFont;

			const auto player_name_strlen = globals::get_canvas()->strlen(font, player_name);

			fvector2d pos = { root_out.x - (player_name_strlen.x / 2.f), root_out.y - 40 };

			globals::get_canvas()->draw_text(font, player_name, pos, { 0.8f, 0.8f }, { 1, 1, 1, 1 }, 1.0f, { 0, 0, 0, 1 }, { 1, 1 }, false, false, true, { 0,0,0,0.4 });
		}

		void draw_skeleton(actor* own_actor, actor* actor)
		{
			int bone_count = actor->get_mesh()->get_num_bones();

			if (bone_count < 101) return;

			bool IsMale = (bone_count == 101 ? false : true); // 101 = Female

			int skeleton[][4] = {
				{ 7, 8, 0, 0},
				{ 7, IsMale ? 49 : 48, IsMale ? 69 : 67, IsMale ? 67 : 69 },
				{ 7, 45, 42, 44 },
				{ 7, 5, 4, 3 },
				{ 3, IsMale ? 90 : 82, IsMale ? 86 : 83, IsMale ? 102 : 85 }, // rechts
				{ 3, IsMale ? 83 : 75, IsMale ? 79 : 76, IsMale ? 101 : 78 }  // links
			};

			flinearcolor color = { 1,0,0,1 };

			auto enc_1 = Encrypt(L"decoy");
			auto enc_2 = Encrypt(L"TrainingBot");
			auto enc_3 = Encrypt(L"targetpracticedummy");

			if (string::contains(system::get_object_name(actor).c_str(), enc_1.decrypt()))
			{
				color = { 0.98,0.011,0.98,1 };
			}
			else if (string::contains(system::get_object_name(actor).c_str(), enc_2.decrypt()) || string::contains(system::get_object_name(actor).c_str(), enc_3.decrypt()))
			{
				return;
			}
			else
			{
				/*if (actor->get_shield() > 0) color = { 0, 0, (float)((actor->get_shield() + 50) / 100), 1 };
				else if (actor->get_health() >= 75) color = { 0, 1, 0, 1 };
				else color = { 1, (float)(actor->get_health() / 100), 0, 1 };*/

				color = { 1, 0, 0, 1 };
			}

			enc_1.encrypt();
			enc_2.encrypt();
			enc_3.encrypt();

			if (actor->is_dormant())
				color = { 0.70, 0.70, 0.70, 0.65 };

			if (global::esp::config.bDrawSkeletonHead)
			{
				float distance_modifier = distance(own_actor->get_location(), actor->get_mesh()->bone(31)) * 0.001f;

				auto head = globals::get_canvas()->project(actor->get_mesh()->bone(31));

				if (!head.z)
					return;

				globals::get_canvas()->draw_circle(head.x - 11.5f, head.y - 35.3f, 5, 60, { global::esp::config.colSkelHeadR, global::esp::config.colSkelHeadG, global::esp::config.colSkelHeadB, 1.0f });
			}

			for (auto part : skeleton)
			{
				fvector previous{};
				for (int i = 0; i < 4; i++)
				{
					if (!part[i])
						break;

					fvector current = actor->get_mesh()->bone(part[i]);

					if (previous.x == 0.f)
					{
						previous = current;
						continue;
					}

					fvector p1{}, c1{};

					p1 = globals::get_canvas()->project(previous);
					c1 = globals::get_canvas()->project(current);

					if (!p1.z || !c1.z)
						return;

					globals::get_canvas()->draw_line({ p1.x, p1.y }, { c1.x, c1.y }, global::esp::config.fSkeletonThickness, color);

					previous = current;
				}
			}
		}

		void draw_wireframe(actor* local_pawn) {
			static bool oldWireFrameGun = false;
			static bool oldWireFrameHand = false;

			if (oldWireFrameGun != global::esp::config.bWireFrameGun) {
				oldWireFrameGun = global::esp::config.bWireFrameGun;

				if (inventory* inventory = local_pawn->get_inventory()) {
					if (equippable* equippable = inventory->get_current_equippable()) {

						auto mesh1p = read<uobject*>(std::uintptr_t(equippable) + offsets::equippable::mesh1p);
						auto bitwise = read<char>(std::uintptr_t(mesh1p) + offsets::mesh::wireframe_bitset);

						static char oldWireframeBitset = -1;

						if (global::esp::config.bWireFrameGun == false) {
							if (oldWireframeBitset != -1) {
								write<char>(std::uintptr_t(mesh1p) + offsets::mesh::wireframe_bitset, 0);
								oldWireframeBitset = -1;
							}

						}
						else {
							oldWireframeBitset = read<char>(std::uintptr_t(mesh1p) + offsets::mesh::wireframe_bitset);
							write<char>(std::uintptr_t(mesh1p) + offsets::mesh::wireframe_bitset, bitwise | (1 << 5));
						}
						write<char>(std::uintptr_t(mesh1p) + 0xc0, 0xff);
						printf("swapped\n");
					}
				}
			}

			if (oldWireFrameHand != global::esp::config.bWireFrameHands) {
				oldWireFrameHand = global::esp::config.bWireFrameHands;
				if (inventory* inventory = local_pawn->get_inventory()) {
					if (equippable* equippable = inventory->get_current_equippable()) {

						auto mesh1p = read<uobject*>(std::uintptr_t(local_pawn) + offsets::actor::overlay1p);
						auto bitwise = read<char>(std::uintptr_t(mesh1p) + offsets::mesh::wireframe_bitset);

						static char oldWireframeBitset = -1;

						if (global::esp::config.bWireFrameHands == false) {
							if (oldWireframeBitset != -1) {
								write<char>(std::uintptr_t(mesh1p) + offsets::mesh::wireframe_bitset, oldWireframeBitset);
								oldWireframeBitset = -1;
							}
						}
						else {
							oldWireframeBitset = read<char>(std::uintptr_t(mesh1p) + offsets::mesh::wireframe_bitset);
							write<char>(std::uintptr_t(mesh1p) + offsets::mesh::wireframe_bitset, bitwise | (1 << 5));
						}
						write<char>(std::uintptr_t(mesh1p) + 0xc0, 0xff); // dirty
					}
				}
			}
		}

		void run()
		{

			if (global::esp::config.bEnabled || global::esp::config.bHotkeyVisuals || global::esp::config.bSkeleton)
			{
				if (controller* controller = blueprint::get_local_controller(globals::get_world_instance()))
				{
					if (actor* pawn = controller->get_pawn())
					{
						draw_wireframe(pawn);
						draw_recoil(pawn, controller);

						if (global::esp::config.iEspType > 0 && global::esp::config.bHotkeyVisuals)
							if (!controller->is_input_key_down(pinguu::keys::convert_key(global::esp::config.iKey)))
								return;

						auto actors = globals::get_players();

						for (int i = 0; i < actors.size(); i++)
						{
							if (actor* actor = actors[i])
							{
								if (actor->is_dormant()) continue;
								if (actor != pawn)
								{
									if (team_component::is_ally(pawn, actor)) continue;

									if (global::esp::config.bSkeleton)
										draw_skeleton(pawn, actor);

									if (global::esp::config.bEnabled || global::esp::config.bHotkeyVisuals)
									{
										draw_esp_box(actor, global::esp::config.iEspType);

										if (global::esp::config.bDrawNames)
											draw_names(actor);
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