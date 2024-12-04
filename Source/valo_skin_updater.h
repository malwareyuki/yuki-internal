#pragma once

class updater {
public:
	static inline void unlock_all_skins(uobject* world) {
		if (inventory_manager* manager = ares_instance::get_ares_client_game_instance(world)->get_inventory_manager()) {
			array<uobject*> equippable_models = read<array<uobject*>>(std::uintptr_t(manager) + 0xe0);

			for (int i = 0; i < equippable_models.size(); i++) {
				if (auto model = equippable_models[i]) {
					array<uobject*> skins = read<array<uobject*>>(std::uintptr_t(model) + 0x150);

					if (skins.size() > 0) {
						for (int a = 0; a < skins.size(); a++) {
							if (auto skin = skins[a]) {

								array<uobject*> possible_levels = read<array<uobject*>>(std::uintptr_t(skin) + 0x138);
								for (int a = 0; a < possible_levels.size(); a++)
									if (uobject* xyz = possible_levels[a])
										write<bool>(std::uintptr_t(xyz) + 0xfe, true),
										write<bool>(std::uintptr_t(xyz) + 0xf2, true);


								array<uobject*> possible_chromas = read<array<uobject*>>(std::uintptr_t(skin) + 0x128); // ?
								for (int a = 0; a < possible_chromas.size(); a++)
									if (uobject* xyz = possible_chromas[a])
										write<bool>(std::uintptr_t(xyz) + 0xfe, true),
										write<bool>(std::uintptr_t(xyz) + 0xf2, true);


								write<bool>(std::uintptr_t(skin) + 0xfe, true);
								//write<bool>(std::uintptr_t(skin) + 0xf3, true);
								write<bool>(std::uintptr_t(skin) + 0xf2, true);
							}
						}
					}
				}
			}

			auto charm_instances = read<array<tmap<uobject*, uobject*>>>(std::uintptr_t(manager) + offsets::inventory_manager::charm_map);
			for (int i = 0; i < charm_instances.size(); i++) {
				if (auto asset = charm_instances[i].Element) {
					write<bool>(std::uintptr_t(asset) + 0xfe, true);
					write<bool>(std::uintptr_t(asset) + 0xf2, true);
				}
			}
		}
	}
};
