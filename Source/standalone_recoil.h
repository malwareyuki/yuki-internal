#pragma once

namespace pinguu
{
	namespace standalone_recoil
	{
		frotator previous_recoil = {};

		static void standalone_recoil(controller* controller)
		{
			if (actor* pawn = controller->get_pawn())
			{
				const frotator& view_angle = controller->get_control_rotation();
				const frotator& recoil = pawn->get_view_rotation_with_full_recoil() - pawn->get_view_rotation_with_no_recoil();
				const frotator& result = ::math::normalized_delta_rotator(view_angle - (recoil - standalone_recoil::previous_recoil), view_angle);

				auto sensitivity = controller->get_mouse_sensitivity();

				if (pawn->is_still_firing())
				{
					controller->add_pitch(result.pitch / -sensitivity);
					controller->add_yaw(result.yaw / sensitivity);
				}

				standalone_recoil::previous_recoil = recoil;
			}
			else standalone_recoil::previous_recoil = {};
		}

		void run()
		{
			if (global::misc::config.bStandaloneRecoil)
			{
				if (controller* controller = blueprint::get_local_controller(globals::get_world_instance()))
					standalone_recoil(controller);
			}
		}
	}
}