namespace offsets
{
	namespace main {
		auto static_find_object = Encrypt("0x3EF68D0");
		auto process_event = Encrypt("0x3EE2230");
		auto bone_matrix = Encrypt("0x598C740");
		auto bypass_skin_changer = Encrypt("0x2D602A0");
		auto set_ares_outline_mode = Encrypt("0x593B6F0");
		auto input_key = Encrypt("0x602D080");

		// sig finds all 3 functions
		auto add_pitch = Encrypt("0x5DCC230"); // 40 53 48 83 EC 30 48 8B 01 48 8B D9 0F 29 74 24 ? 0F 28 F1 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 84 C0 75 0A F3 0F 59 B3 ? ? ? ? EB 03
		auto add_yaw = Encrypt("0x5DCCBE0"); // 40 53 48 83 EC 30 48 8B 01 48 8B D9 0F 29 74 24 ? 0F 28 F1 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 84 C0 75 0A F3 0F 59 B3 ? ? ? ? EB 03

		uintptr_t view_port = 0x8c0;
		uintptr_t post_render = 0x6A;
	}

	namespace actor {
		uintptr_t dormant = 0x100; // N/A
		uintptr_t mesh_3p_mids = 0xcc0; // struct TArray<struct UMaterialInstanceDynamic*> Mesh3PMIDs; // 0xcb0(0x10)
		uintptr_t fresnel_component = 0x8a8; // struct UFresnelComponent* FresnelComponent; // 0x898(0x08)
		uintptr_t overlay1p = 0xc70; // struct USkeletalMeshComponent* MeshOverlay1P; // 0xc60(0x08)
		uintptr_t abilities_and_buffs = 0x848; // struct UAresAbilitySystemComponent* AbilitiesAndBuffs; // 0x838(0x08)
		uintptr_t cached_attribute_set = 0x1448; // struct UAresAttributeSet* CachedAttributeSet; // 0x1448(0x08)
	}

	namespace cached_attribute_set {
		uintptr_t weapons_lowered = 0x5f8; // struct FGameplayBooleanAttributeData WeaponsLowered; // 0x5f8(0x10)
	}

	namespace mesh {
		uintptr_t wireframe_bitset = 0x71e; // 	char bForceWireframe : 1; // 0x71e(0x01)
	}

	namespace equippable {
		uintptr_t mesh1p = 0xcd0; // struct USkeletalMeshComponent* Mesh1P; // 0xcc8(0x08)
	}

	namespace fresnel_component {
		uintptr_t fresnel_intensity = 0x1c4; // float FresnelIntensity; // 0x1c4(0x04) | oder | float OriginalFresnelIntensity; // 0x1cc(0x04)
		// oder wir nutzen 	void SetIntensity(float NewIntensity); // Function ShooterGame.FresnelComponent.SetIntensity // (Final|Native|Protected|BlueprintCallable) // @ game+0x36af230
	}

	namespace radar {
		uintptr_t portrait_minimap = 0x1198; // struct UCharacterPortraitMinimapComponent_C* CharacterPortraitMinimapComponent; // 0x1188(0x08)
		uintptr_t character_minimap = portrait_minimap + 0x8; // struct UShooterCharacterMinimapComponent_C* ShooterCharacterMinimapComponent; // 0x1190(0x08)
	}

	namespace portrait_minimap {
		uintptr_t character_icon = 0x548; // struct UTexture* CharacterIcon; // 0x548(0x08)
	}

	namespace controller {
		uintptr_t player_input = 0x4f0; // struct UPlayerInput* PlayerInput; // 0x4e8(0x08)
		uintptr_t pregame_view_controller = 0x9f0; // struct UPregameViewController* PregameViewController; // 0x9d0(0x08)
		uintptr_t show_mouse_cursor = 0x5f8; // char bShowMouseCursor : 1; // 0x5f0(0x01)
	}

	namespace minimap_component {
		uintptr_t observer_status = 0x530; // bool bLocalObserver; // 0x530(0x01) (UShooterCharacterMinimapComponent)
		uintptr_t visible_status = 0x501; // bool bIsVisible; // 0x501(0x01) (UShooterCharacterMinimapComponent)
	}

	namespace canvas {
		uintptr_t size_x = 0x48; // int32_t SizeX; // 0x48(0x04)
		uintptr_t size_y = 0x4c; // int32_t SizeY; // 0x4c(0x04)
	}

	namespace engine {
		uintptr_t medium_font = 0x88; // struct UFont* MediumFont; // 0x88(0x08)
		uintptr_t subtitle_font = 0xD8; // struct UFont* SubtitleFont; // 0xd8(0x08)
	}

	namespace viewport_client {
		uintptr_t world = 0x80; // struct UWorld* World; // 0x80(0x08)
		uintptr_t game_instance = 0x88; // struct UGameInstance* GameInstance; // 0x88(0x08)
	}

	namespace bomb_offsets {
		uintptr_t time_remaining_to_explode = 0x4e4; // float TimeRemainingToExplode; // 0x4dc(0x04)
		uintptr_t defuse_progress = 0x510; // float DefuseProgress; // 0x508(0x04)
	}

	namespace inventory_manager {
		uintptr_t charm_map = 0x430; // struct TMap<struct UEquippableCharmDataAsset*, struct UEquippableCharmInventoryModel*> DataAssetToEquippableCharmMap; // 0x430(0x50)
	}
};