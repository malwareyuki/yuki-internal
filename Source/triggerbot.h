#pragma once

namespace pinguu {
	namespace triggerbot {
		namespace defines {
			bool shot_out = false;
			int old_time = NULL;
			int old_time_anyway = NULL;
		}

		void run() {
			if (global::triggerbot::config.bEnabled)
				if (controller* controller = blueprint::get_local_controller(globals::get_world_instance()))
					if (controller->is_input_key_down(pinguu::keys::convert_key(global::triggerbot::config.iKey)))
					{
						if (!defines::old_time_anyway) // 	float GetWorldDeltaSeconds(struct UObject* WorldContextObject); // Function Engine.GameplayStatics.GetWorldDeltaSeconds // (Final|Native|Static|Public|BlueprintCallable|BlueprintPure) // @ game+0x5145350

							defines::old_time_anyway = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

						int current_time_anyway = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

						if (current_time_anyway > defines::old_time_anyway) {
							defines::old_time_anyway = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + global::triggerbot::config.iDelay;

							if (actor* local_pawn = controller->get_pawn())
								if (actor* tracked_actor = local_pawn->simple_trace_from_camera(9999, collision_channel::ECC_GameTraceChannel1, nullptr, nullptr))
									if (math::class_is_child_of(tracked_actor->class_private, classes::filters::character))
										if (!team_component::get_team_component(local_pawn)->is_ally(local_pawn, tracked_actor))
										{
											if (global::triggerbot::config.bShotOnce) {
												if (defines::shot_out)
													return;

												controller->simulate_input_key(keys::left_mouse_button, true);
												controller->simulate_input_key(keys::left_mouse_button, false);

												defines::shot_out = true;
											}
											else {

												controller->simulate_input_key(keys::left_mouse_button, true);
												controller->simulate_input_key(keys::left_mouse_button, false);

											}
										}
						}
					}
					else if (defines::shot_out)
						defines::shot_out = false;
		}
	}
}