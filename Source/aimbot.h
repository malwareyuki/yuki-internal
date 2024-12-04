#pragma once

namespace pinguu
{
	namespace aimbot
	{
		namespace math
		{
			frotator interpolate(const float speed, const frotator& current, const frotator& target, float delta_time)
			{
				if (global::aimbot::config.bConstant)
					return sdk::valorant::math::rotator_interp_to_constant(current, target, delta_time, speed);
				else
					return sdk::valorant::math::rotator_interp_to(current, target, delta_time, speed);
			}
		}

		void run(controller* controller)
		{
			if (global::aimbot::config.bEnabled)
			{
				auto actors = globals::get_players();
				if (actors.size()) {
					if (actor* pawn = controller->get_pawn()) {
						actor* target = nullptr;
						fvector target_location;

						if (camera* camera = controller->get_camera()) {

							if (inventory* inventory = pawn->get_inventory()) {

								if (equippable* equippable = inventory->get_current_equippable()) {
									if (equippable->should_present_as_usable()) return;

									float fov = global::aimbot::config.fMaxFov;

									const frotator& view_rotation_with_full_recoil = pawn->get_view_rotation_with_full_recoil();
									const frotator& view_rotation_with_no_recoil = pawn->get_view_rotation_with_no_recoil();

									std::int32_t bone = -1;

									for (int i = 0; i < actors.size(); i++) {
										if (auto actor = actors[i]) {
											if (auto mesh = actor->get_mesh()) {
												if (!actor->is_alive() || actor->is_dormant() || team_component::get_team_component(actor)->is_ally(pawn, actor)) continue;

												fvector out_points;
												auto has_line_of_sight = characters_funciton_library::has_line_of_sight_to_character(pawn, actor, collision_channel::ECC_Pawn, globals::get_world_instance(), &out_points);
												if (!has_line_of_sight) continue;

												const fvector& camera_location = camera->get_location();
												const frotator& view_rotation_with_full_recoil = pawn->get_view_rotation_with_full_recoil();

												if (global::aimbot::config.bNearestVisibleLocation)
												{
													if (const float current_fov = sdk::valorant::math::vector_size(sdk::valorant::math::find_look_at_rotation(camera_location, out_points) - view_rotation_with_full_recoil); current_fov < fov) {
														fov = current_fov;
														target = actor;
														target_location = out_points;
													}
												}
												else
												{
													if (global::aimbot::config.bNearestBone) {
														std::array<std::int32_t, 14> bones = { 8, 7, 6, 4, 1, 36, 11, 64, 78, 13, 38, 60, 80, 66 };
														for (const std::int32_t& index : bones) {
															auto location = mesh->bone(index);
															if (const float current_fov = sdk::valorant::math::vector_size(sdk::valorant::math::find_look_at_rotation(camera_location, location) - view_rotation_with_full_recoil); current_fov < fov) {
																fov = current_fov;
																target = actor;
																target_location = location;
															}
														}
													} 
													else {
														auto location = mesh->bone(global::aimbot::config.iHitbox);
														if (const float current_fov = sdk::valorant::math::vector_size(sdk::valorant::math::find_look_at_rotation(camera_location, location) - view_rotation_with_full_recoil); current_fov < fov) {
															fov = current_fov;
															target = actor;
															target_location = location;
														}
													}
												}
											}
										}
										else globals::get_players().clear();
									}

									if (target) {
										if (auto mesh = target->get_mesh()) {
											fvector camera_location = camera->get_location();

											auto enc_1 = Encrypt(L"basepistol");
											auto enc_2 = Encrypt(L"lugerpistol");
											auto enc_3 = Encrypt(L"revolverpistol");
											auto enc_4 = Encrypt(L"automaticshotgun");

											if ((string::contains(system::get_object_name(equippable).c_str(), enc_1.decrypt()) || string::contains(system::get_object_name(equippable).c_str(), enc_2.decrypt()) ||
												string::contains(system::get_object_name(equippable).c_str(), enc_3.decrypt()) || string::contains(system::get_object_name(equippable).c_str(), enc_4.decrypt())) && global::aimbot::config.bAlternatePistoleKey)
											{
												if (controller->is_input_key_down(pinguu::keys::right_mouse_button))
												{
													const auto angle = sdk::valorant::math::find_look_at_rotation(camera_location, target_location) - (view_rotation_with_full_recoil - view_rotation_with_no_recoil) * global::aimbot::config.fRecoilStrength;

													frotator rotator = aimbot::math::interpolate(global::aimbot::config.fSpeed, controller->get_control_rotation(), angle, gameplay_statics::get_world_delta_seconds(controller));

													const frotator& delta = sdk::valorant::math::normalized_delta_rotator(rotator, controller->get_control_rotation());
													auto sensitivity = controller->get_mouse_sensitivity();

													controller->add_pitch(delta.pitch / -sensitivity);
													controller->add_yaw(delta.yaw / sensitivity);
												}
											}
											else
											{
												if (controller->is_input_key_down(keys::convert_key(global::aimbot::config.iKey)))
												{
													const auto angle = sdk::valorant::math::find_look_at_rotation(camera_location, target_location) - (view_rotation_with_full_recoil - view_rotation_with_no_recoil) * global::aimbot::config.fRecoilStrength;

													frotator rotator = aimbot::math::interpolate(global::aimbot::config.fSpeed, controller->get_control_rotation(), angle, gameplay_statics::get_world_delta_seconds(controller));

													const frotator& delta = sdk::valorant::math::normalized_delta_rotator(rotator, controller->get_control_rotation());
													auto sensitivity = controller->get_mouse_sensitivity();

													controller->add_pitch(delta.pitch / -sensitivity);
													controller->add_yaw(delta.yaw / sensitivity);
												}
											}
											enc_1.encrypt(), enc_2.encrypt(), enc_3.encrypt(), enc_4.encrypt();
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
}