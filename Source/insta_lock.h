#pragma once

namespace pinguu
{
	namespace insta_lock_agents
	{
		uobject* astra = nullptr;
		uobject* breach = nullptr;
		uobject* brimstone = nullptr;
		uobject* chamber = nullptr;
		uobject* cypher = nullptr;
		uobject* fade = nullptr;
		uobject* jett = nullptr;
		uobject* kayo = nullptr;
		uobject* killjoy = nullptr;
		uobject* neon = nullptr;
		uobject* omen = nullptr;
		uobject* phoenix = nullptr;
		uobject* raze = nullptr;
		uobject* reyna = nullptr;
		uobject* sage = nullptr;
		uobject* skye = nullptr;
		uobject* sova = nullptr;
		uobject* viper = nullptr;
		uobject* yoru = nullptr;
		uobject* gekko = nullptr;

		void run()
		{
			auto enc_astra = Encrypt(L"Default__Rift_PrimaryAsset_C");
			astra = uobject::find_object(enc_astra.decrypt(), reinterpret_cast<uobject*>(-1)), enc_astra.clear();

			auto enc_breach = Encrypt(L"Default__Breach_PrimaryAsset_C");
			breach = uobject::find_object(enc_breach.decrypt(), reinterpret_cast<uobject*>(-1)), enc_breach.clear();

			auto enc_brimstone = Encrypt(L"Default__Sarge_PrimaryAsset_C");
			brimstone = uobject::find_object(enc_brimstone.decrypt(), reinterpret_cast<uobject*>(-1)), enc_brimstone.clear();

			auto enc_chamber = Encrypt(L"Default__Deadeye_PrimaryAsset_C");
			chamber = uobject::find_object(enc_chamber.decrypt(), reinterpret_cast<uobject*>(-1)), enc_chamber.clear();

			auto enc_cypher = Encrypt(L"Default__Gumshoe_PrimaryAsset_C");
			cypher = uobject::find_object(enc_cypher.decrypt(), reinterpret_cast<uobject*>(-1)), enc_cypher.clear();

			auto enc_fade = Encrypt(L"Default__BountyHunter_PrimaryAsset_C");
			fade = uobject::find_object(enc_fade.decrypt(), reinterpret_cast<uobject*>(-1)), enc_fade.clear();

			auto enc_jett = Encrypt(L"Default__Wushu_PrimaryAsset_C");
			jett = uobject::find_object(enc_jett.decrypt(), reinterpret_cast<uobject*>(-1)), enc_jett.clear();

			auto enc_kayo = Encrypt(L"Default__Grenadier_PrimaryAsset_C");
			kayo = uobject::find_object(enc_kayo.decrypt(), reinterpret_cast<uobject*>(-1)), enc_kayo.clear();

			auto enc_killjoy = Encrypt(L"Default__Killjoy_PrimaryAsset_C");
			killjoy = uobject::find_object(enc_killjoy.decrypt(), reinterpret_cast<uobject*>(-1)), enc_killjoy.clear();

			auto enc_neon = Encrypt(L"Default__Sprinter_PrimaryAsset_C");
			neon = uobject::find_object(enc_neon.decrypt(), reinterpret_cast<uobject*>(-1)), enc_neon.clear();

			auto enc_omen = Encrypt(L"Default__Wraith_PrimaryAsset_C");
			omen = uobject::find_object(enc_omen.decrypt(), reinterpret_cast<uobject*>(-1)), enc_omen.clear();

			auto enc_phoenix = Encrypt(L"Default__Phoenix_PrimaryAsset_C");
			phoenix = uobject::find_object(enc_phoenix.decrypt(), reinterpret_cast<uobject*>(-1)), enc_phoenix.clear();

			auto enc_raze = Encrypt(L"Default__Clay_PrimaryAsset_C");
			raze = uobject::find_object(enc_raze.decrypt(), reinterpret_cast<uobject*>(-1)), enc_raze.clear();

			auto enc_reyna = Encrypt(L"Default__Vampire_PrimaryAsset_C");
			reyna = uobject::find_object(enc_reyna.decrypt(), reinterpret_cast<uobject*>(-1)), enc_reyna.clear();

			auto enc_sage = Encrypt(L"Default__Thorne_PrimaryAsset_C");
			sage = uobject::find_object(enc_sage.decrypt(), reinterpret_cast<uobject*>(-1)), enc_sage.clear();

			auto enc_skye = Encrypt(L"Default__guide_PrimaryAsset_C");
			skye = uobject::find_object(enc_skye.decrypt(), reinterpret_cast<uobject*>(-1)), enc_skye.clear();

			auto enc_sova = Encrypt(L"Default__Hunter_PrimaryAsset_C");
			sova = uobject::find_object(enc_sova.decrypt(), reinterpret_cast<uobject*>(-1)), enc_sova.clear();

			auto enc_viper = Encrypt(L"Default__Pandemic_PrimaryAsset_C");
			viper = uobject::find_object(enc_viper.decrypt(), reinterpret_cast<uobject*>(-1)), enc_viper.clear();

			auto enc_yoru = Encrypt(L"Default__Stealth_PrimaryAsset_C");
			yoru = uobject::find_object(enc_yoru.decrypt(), reinterpret_cast<uobject*>(-1)), enc_yoru.clear();

			auto enc_gekko = Encrypt(L"Default__AggroBot_PrimaryAsset_C");
			gekko = uobject::find_object(enc_gekko.decrypt(), reinterpret_cast<uobject*>(-1)), enc_gekko.clear();
		}

		uobject* get_agent_object(int id)
		{
			if (id == 0) return astra;
			else if (id == 1) return breach;
			else if (id == 2) return brimstone;
			else if (id == 3) return chamber;
			else if (id == 4) return cypher;
			else if (id == 5) return fade;
			else if (id == 6) return jett;
			else if (id == 7) return kayo;
			else if (id == 8) return killjoy;
			else if (id == 9) return neon;
			else if (id == 10) return omen;
			else if (id == 11) return phoenix;
			else if (id == 12) return raze;
			else if (id == 13) return reyna;
			else if (id == 14) return sage;
			else if (id == 15) return skye;
			else if (id == 16) return sova;
			else if (id == 17) return viper;
			else if (id == 18) return yoru;
			else if (id == 19) return gekko;
			else return nullptr;
		}
	}

	namespace insta_lock
	{
		int old_time = NULL;
		int agent_counter = 0;

		void agent_roulette(controller* controller) {
			auto _class = find_obj(L"ShooterGame.PregamePlayerController");
			if (math::class_is_child_of(controller->class_private, _class))
				if (pregame_view_controller* pregame_view_controller = controller->get_pregame_view_controller())
					if (pregame_view_model* pregame_view_model = pregame_view_controller->get_pregame_view_model())
						if (!pregame_view_model->is_local_player_locked_in())
						{
							if (!old_time) old_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
							int current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

							if (current_time > old_time)
							{
								old_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + 50;
								pregame_view_controller->select_character(insta_lock_agents::get_agent_object(agent_counter));

								agent_counter++;
								if (agent_counter >= 20)
									agent_counter = 0;

							}
						}
						else if (global::misc::config.bRandomAgent == true)
							global::misc::config.bRandomAgent = false;
		}

		void run(controller* controller)
		{
			if (global::misc::config.bRandomAgent)
				agent_roulette(controller);

			if (global::misc::config.bInstaLock) {
				auto _class = find_obj(L"ShooterGame.PregamePlayerController");
				if (math::class_is_child_of(controller->class_private, _class))
					if (pregame_view_controller* pregame_view_controller = controller->get_pregame_view_controller())
						if (pregame_view_model* pregame_view_model = pregame_view_controller->get_pregame_view_model())
							if (!pregame_view_model->is_local_player_locked_in())
								if (insta_lock_agents::get_agent_object(global::misc::config.iInstaLockAgent))
								{
									pregame_view_controller->lock_character(insta_lock_agents::get_agent_object(global::misc::config.iInstaLockAgent));
									global::misc::config.bInstaLock = false;
								}

			}
		}
	}
}