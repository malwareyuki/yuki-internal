#pragma once

namespace pinguu
{
	namespace skin_changer_utils
	{
		equippable_inventory_model* get_aresnal_gun_models(int type_index)
		{
			if (controller* controller = blueprint::get_first_local_player_controller(globals::get_world_instance()))
				if (ares_client_game_instace* ares = ares_instance::get_ares_client_game_instance(globals::get_world_instance()))
					if (arsenal_view_controller* arsenal_view_controller = ares->get_aresnal_view_controller())
						if (arsenal_view_model* arsenal_view_model = arsenal_view_controller->get_view_model())
						{
							auto models = arsenal_view_model->get_gun_models();
							for (int i = 0; i < models.size(); i++)
							{
								auto model = models[i];

								if (equippable_skin_data_asset* skin_data = model->get_equipped_skin_model()->get_skin_data_asset())
								{
									if (equippable_skin_data_asset::type type = skin_data->get_type(); type.is_valid())
									{
										if (type.index == type_index)
										{
											return model;
										}
									}
								}
							}
						}
			return nullptr;
		}
	}

	namespace skin_changer
	{
		void skin_changer(controller* controller)
		{
			if (actor* pawn = controller->get_pawn())
			{
				if (inventory* inventory = pawn->get_inventory())
				{
					if (equippable* equippable = inventory->get_current_equippable())
					{
						if (equippable::type type = equippable->get_type(); type.is_valid())
						{
							if (equippable_skin_data_asset* current_real_skin = equippable->get_equippable_skin_data_asset())
							{
								if (equippable_inventory_model* model = skin_changer_utils::get_aresnal_gun_models(type.index))
								{
									int max_level = model->get_equipped_skin_model()->get_skin_data_asset()->get_skin_levels().size();
									uobject* skin_data_asset = model->get_equipped_skin_model()->get_skin_data_asset();
									uobject* skin_chroma_asset = model->get_equipped_skin_model()->get_skin_inventory_chroma_asset()->get_skin_chroma_data_asset();
									charm_data_asset* skin_charm_data_asset = model->get_equipped_charm_instance()->get_charm_data_asset();
									int skin_charm_level_data_asset = model->get_equipped_charm_instance()->get_charm_level_data_asset()->get_charm_level();

									auto re_ = read<uobject*>(std::uintptr_t(equippable) + 0xdc8);
									auto name = system::get_object_name(re_);
									auto enc = Encrypt(L"standard");
									if (!skin_data_asset || !skin_chroma_asset || (uobject*)skin_data_asset == current_real_skin || !string::contains(name.c_str(), enc.decrypt())) {
										enc.encrypt();
										return;
									}
									enc.encrypt();

									write(std::uintptr_t(equippable) + 0xdc8, skin_data_asset);
									write(std::uintptr_t(equippable) + 0xde8, skin_chroma_asset);

									if (auto decrypted = system::decrypt_parent(equippable)) {
										auto a1 = read<uintptr_t>(decrypted + 0x318);
										a1 = read<uintptr_t>(a1 + 0xA0);

										auto old = read<int>(a1 + 0x80);
										write<int>(a1 + 0x80, 2);

										if (!skin_charm_data_asset) {
											if (inventory_manager* manager = ares_instance::get_ares_client_game_instance(globals::get_world_instance())->get_inventory_manager()) {
												auto charm_instances = read<array<tmap<uobject*, uobject*>>>(std::uintptr_t(manager) + offsets::inventory_manager::charm_map);
												skin_charm_data_asset = (charm_data_asset*)charm_instances[global::skin_changer::config.iGunBuddy].Key;
											}
										}

										content_library::clear_weapon_components(equippable);
										content_library::apply_skin(equippable, skin_data_asset, skin_chroma_asset, max_level, skin_charm_data_asset, type.index == 17 ? -1 : 1);

										write<int>(a1 + 0x80, old);
									}
								}
							}
						}
					}
				}
			}
		}

		void run() {
			if (global::skin_changer::config.bEnabled)
				if (controller* controller = blueprint::get_local_controller(globals::get_world_instance()))
					skin_changer(controller);
		}
	}
}