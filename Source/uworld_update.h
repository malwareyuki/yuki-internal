#pragma once
class world_finder {

public:
	static inline uobject* find_uworld() {
		auto enc = Encrypt(L"/Game/Maps/Init/Init.Init");
		if (uobject* world = uobject::find_object(enc.decrypt(), reinterpret_cast<uobject*>(-1))) {
			enc.encrypt();
			return world;
		}
		else {
			auto enc = Encrypt(L"/Game/Maps/Menu/MainMenuV2.MainMenuV2");
			if (uobject* world = uobject::find_object(enc.decrypt(), reinterpret_cast<uobject*>(-1))) {
				enc.encrypt();
				return world;
			}
			else {
				auto enc = Encrypt(L"/Game/Maps/Poveglia/Range.Range");
				if (uobject* world = uobject::find_object(enc.decrypt(), reinterpret_cast<uobject*>(-1))) {
					enc.encrypt();
					return world;
				}
				else {
					auto enc = Encrypt(L"/Game/Maps/Ascent/Ascent.Ascent");
					if (uobject* world = uobject::find_object(enc.decrypt(), reinterpret_cast<uobject*>(-1))) {
						enc.encrypt();
						return world;
					}
				}
			}
		}


		return nullptr;
	}
};