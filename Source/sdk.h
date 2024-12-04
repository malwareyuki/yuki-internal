#pragma once

/*
	@ created in multiple years, should be cleaned up xd
*/

#define find_obj(value) [&](){ \
	static uobject* function; \
	if (!function) { \
		auto enc = Encrypt(value); \
		function = uobject::find_object(enc.decrypt()), enc.clear(); \
	} \
	return function; }();

enum collision_channel : std::uint8_t {
	ECC_WorldStatic = 0,
	ECC_WorldDynamic = 1,
	ECC_Pawn = 2,
	ECC_Visibility = 3,
	ECC_Camera = 4,
	ECC_PhysicsBody = 5,
	ECC_Vehicle = 6,
	ECC_Destructible = 7,
	ECC_EngineTraceChannel1 = 8,
	ECC_EngineTraceChannel2 = 9,
	ECC_EngineTraceChannel3 = 10,
	ECC_EngineTraceChannel4 = 11,
	ECC_EngineTraceChannel5 = 12,
	ECC_EngineTraceChannel6 = 13,
	ECC_GameTraceChannel1 = 14,
	ECC_GameTraceChannel2 = 15,
	ECC_GameTraceChannel3 = 16,
	ECC_GameTraceChannel4 = 17,
	ECC_GameTraceChannel5 = 18,
	ECC_GameTraceChannel6 = 19,
	ECC_GameTraceChannel7 = 20,
	ECC_GameTraceChannel8 = 21,
	ECC_GameTraceChannel9 = 22,
	ECC_GameTraceChannel10 = 23,
	ECC_GameTraceChannel11 = 24,
	ECC_GameTraceChannel12 = 25,
	ECC_GameTraceChannel13 = 26,
	ECC_GameTraceChannel14 = 27,
	ECC_GameTraceChannel15 = 28,
	ECC_GameTraceChannel16 = 29,
	ECC_GameTraceChannel17 = 30,
	ECC_GameTraceChannel18 = 31,
	ECC_OverlapAll_Deprecated = 32,
	ECC_MAX = 33
};

enum class object_types : std::uint8_t {
	ObjectTypeQuery1 = 0,
	ObjectTypeQuery2 = 1,
	ObjectTypeQuery3 = 2,
	ObjectTypeQuery4 = 3,
	ObjectTypeQuery5 = 4,
	ObjectTypeQuery6 = 5,
	ObjectTypeQuery7 = 6,
	ObjectTypeQuery8 = 7,
	ObjectTypeQuery9 = 8,
	ObjectTypeQuery10 = 9,
	ObjectTypeQuery11 = 10,
	ObjectTypeQuery12 = 11,
	ObjectTypeQuery13 = 12,
	ObjectTypeQuery14 = 13,
	ObjectTypeQuery15 = 14,
	ObjectTypeQuery16 = 15,
	ObjectTypeQuery17 = 16,
	ObjectTypeQuery18 = 17,
	ObjectTypeQuery19 = 18,
	ObjectTypeQuery20 = 19,
	ObjectTypeQuery21 = 20,
	ObjectTypeQuery22 = 21,
	ObjectTypeQuery23 = 22,
	ObjectTypeQuery24 = 23,
	ObjectTypeQuery25 = 24,
	ObjectTypeQuery26 = 25,
	ObjectTypeQuery27 = 26,
	ObjectTypeQuery28 = 27,
	ObjectTypeQuery29 = 28,
	ObjectTypeQuery30 = 29,
	ObjectTypeQuery31 = 30,
	ObjectTypeQuery32 = 31,
	ObjectTypeQuery_MAX = 32,
	EObjectTypeQuery_MAX = 33
};

enum class debug_trace_channel : std::uint8_t {
	None = 0,
	ForOneFrame = 1,
	ForDuration = 2,
	Persistent = 3,
	EDrawDebugTrace_MAX = 4
};


namespace pinguu {
	namespace sdk {
		namespace valorant {
			enum class outline_mode : std::uint8_t {
				none, outline, block, enemy, always_outline, always_enemy
			};

			namespace classes {
				static inline uobject* gameplay_statics = nullptr;
				static inline uobject* shooter_blueprint_library = nullptr;
				static inline uobject* team_component = nullptr;

				static inline uobject* system = nullptr;
				static inline uobject* string = nullptr;
				static inline uobject* text = nullptr;
				static inline uobject* input = nullptr;
				static inline uobject* math = nullptr;
				static inline uobject* blueprint = nullptr;
				static inline uobject* ares_instance = nullptr;
				static inline uobject* outline_rendering = nullptr;
				static inline uobject* render_lib = nullptr;

				static inline uobject* content_library = nullptr;
				static inline uobject* character_function_library = nullptr;

				static inline uobject* ares_class = nullptr;

				namespace filters {
					static inline uclass* character = nullptr;
					static inline uclass* controller = nullptr;
					static inline uclass* local_player = nullptr;
					static inline uclass* projectile = nullptr;
					static inline uclass* actors = nullptr;
					static inline uclass* gun_models = nullptr;
				}

				static inline void initialize() {
					classes::gameplay_statics = find_obj(L"Engine.Default__GameplayStatics");
					classes::team_component = find_obj(L"ShooterGame.Default__BaseTeamComponent");
					classes::system = find_obj(L"Engine.Default__KismetSystemLibrary");
					classes::string = find_obj(L"Engine.Default__KismetStringLibrary");
					classes::text = find_obj(L"Engine.Default__KismetTextLibrary");
					classes::input = find_obj(L"Engine.Default__KismetInputLibrary");
					classes::math = find_obj(L"Engine.Default__KismetMathLibrary");
					classes::blueprint = find_obj(L"ShooterGame.Default__ShooterBlueprintLibrary");
					classes::content_library = find_obj(L"ShooterGame.Default__ContentLibrary");
					classes::ares_instance = find_obj(L"ShooterGame.Default__AresClientGameInstance");
					classes::outline_rendering = find_obj(L"Renderer.Default__AresOutlineRendering");
					classes::ares_class = find_obj(L"ShooterGame.Default__AresOutlineComponent");
					classes::render_lib = find_obj(L"Engine.Default__KismetRenderingLibrary");

					auto enc = Encrypt(L"Ares_Characters_FunctionLibrary.Default__Ares_Characters_FunctionLibrary_C");
					classes::character_function_library = uobject::find_object(enc.decrypt(), reinterpret_cast<uobject*>(-1)), enc.clear();

					auto character_filter = Encrypt(L"ShooterGame.ShooterCharacter");
					auto controller_filter = Encrypt(L"ShooterGame.ShooterPlayerController");
					auto gun_models_filter = Encrypt(L"ShooterGame.EquippableInventoryModel");

					classes::filters::character = uobject::find_object<uclass>(character_filter.decrypt()), character_filter.clear();
					classes::filters::controller = uobject::find_object<uclass>(controller_filter.decrypt()), controller_filter.clear();
					classes::filters::gun_models = uobject::find_object<uclass>(gun_models_filter.decrypt()), gun_models_filter.clear();
				}
			};

			class characters_funciton_library {
			public:
				static inline bool has_line_of_sight_to_character(uobject* viewer, uobject* target, collision_channel trace_channel, uobject* world_context, fvector* focus_point) {
					static uobject* function;
					if (!function) {
						auto enc = Encrypt(L"Ares_Characters_FunctionLibrary.Ares_Characters_FunctionLibrary_C.HasLineOfSightToCharacter");
						function = uobject::find_object(enc.decrypt(), reinterpret_cast<uobject*>(-1)), enc.clear();
					}

					struct {
						uobject* viewer;
						uobject* target;
						collision_channel trace_channel;
						uobject* world_context;
						bool has_visibility;
						fvector focus_point;
					} params = { viewer, target, trace_channel, world_context };

					classes::character_function_library->process_event(function, &params);

					if (focus_point)
						*focus_point = params.focus_point;

					return params.has_visibility;
				}


			};

			class rendering_lib {
			public:
				static inline uobject* import_file_as_texture(uobject* world_context, fstring file_name)
				{
					uobject* function = find_obj(L"Engine.KismetRenderingLibrary.ImportFileAsTexture2D");

					struct
					{
						uobject* world_context;
						fstring file_name;
						uobject* return_value;
					} params = { world_context, file_name };

					classes::render_lib->process_event(function, &params);

					return params.return_value;
				}
			};

			class system {
			public:
				static inline uintptr_t decrypt_parent(uobject* parent) {
					static __int64(__fastcall * decrypt_parent)(__int64);
					if (!decrypt_parent) {
						int retval = strtol(offsets::main::bypass_skin_changer.decrypt(), NULL, 16);
						decrypt_parent = ((__int64(__fastcall*)(__int64 a1))(memory::get_base_address() + retval));
						offsets::main::bypass_skin_changer.clear();
					}
					return decrypt_parent((__int64)parent);
				}

				static inline bool raycast(uobject* world, fvector start, fvector end)
				{
					uobject* function = find_obj(L"Engine.KismetSystemLibrary.LineTraceSingle");

					if (function == nullptr) return false;

					struct
					{
						uobject* WorldContextObject;
						fvector Start;
						fvector End;
						char TraceChannel;
						bool bTraceComplex;
						array<void*> ActorsToIgnore;
						char DrawDebugType;
						fhitresult OutHit;
						bool bIgnoreSelf;
						flinearcolor TraceColor;
						flinearcolor TraceHitColor;
						float DrawTime;

						bool ReturnValue;
					} params = {};

					fhitresult res;

					params.WorldContextObject = world;
					params.Start = start;
					params.End = end;
					params.TraceChannel = 0;
					params.bTraceComplex = true;
					params.ActorsToIgnore = {};
					params.DrawDebugType = 0;
					params.bIgnoreSelf = true;
					params.TraceColor = {};
					params.TraceHitColor = {};
					params.DrawTime = 0.f;

					classes::system->process_event(function, &params);

					res = params.OutHit;

					return !params.ReturnValue;
				}

				static inline fstring get_unique_device_id()
				{
					uobject* function = find_obj(L"Engine.KismetSystemLibrary.GetUniqueDeviceId");

					if (function == nullptr) return nullptr;

					struct
					{
						fstring return_value;
					} params;

					classes::system->process_event(function, &params);

					return params.return_value;
				}

				static inline fstring get_device_id()
				{
					uobject* function = find_obj(L"Engine.KismetSystemLibrary.GetDeviceId");

					if (function == nullptr) return nullptr;

					struct
					{
						fstring return_value;
					} params;

					classes::system->process_event(function, &params);

					return params.return_value;
				}

				static inline fstring get_object_name(uobject* object) {

					uobject* function = find_obj(L"Engine.KismetSystemLibrary.GetObjectName");

					struct {
						uobject* object;
						fstring return_value;
					} params = { object };

					classes::system->process_event(function, &params);

					return params.return_value;
				}

				static inline fstring get_path_name(uobject* object) {
					uobject* function = find_obj(L"Engine.KismetSystemLibrary.GetPathName");

					if (function == nullptr)
						return {};

					struct
					{
						uobject* object;
						fstring return_value;
					} params;

					params.object = object;

					classes::system->process_event(function, &params);

					return params.return_value;
				}

				static inline fstring get_class_display_name(uobject* object) {
					uobject* function = find_obj(L"Engine.KismetSystemLibrary.GetClassDisplayName");

					if (function == nullptr)
						return {};

					struct
					{
						uobject* object;
						fstring return_value;
					} params;

					params.object = object;

					classes::system->process_event(function, &params);

					return params.return_value;
				}

				static inline uobject* get_outer_object(uobject* object) {
					uobject* function = find_obj(L"Engine.KismetSystemLibrary.GetOuterObject");

					if (function == nullptr)
						return nullptr;

					struct
					{
						uobject* object;
						uobject* return_value;
					} params;

					params.object = object;

					classes::system->process_event(function, &params);

					return params.return_value;
				}

				static inline bool is_valid(uobject* object) {
					uobject* function = find_obj(L"Engine.KismetSystemLibrary.IsValid");

					if (function == nullptr)
						return false;

					struct
					{
						uobject* object;
						bool return_value;
					} params;

					params.object = object;

					classes::system->process_event(function, &params);

					return params.return_value;
				}

				static inline bool is_valid_class(uobject* classs) {
					uobject* function = find_obj(L"Engine.KismetSystemLibrary.IsValidClass");

					if (function == nullptr)
						return false;

					struct
					{
						uobject* object;
						bool return_value;
					} params;

					params.object = classs;

					classes::system->process_event(function, &params);

					return params.return_value;
				}
			};

			class string {
			public:
				enum search_case : std::uint8_t {
					case_sensitive, ignore_case
				};

				static inline fstring int_to_string(const std::int32_t value) {
					uobject* function = find_obj(L"Engine.KismetStringLibrary.Conv_IntToString");

					if (function == nullptr)
						return {};

					struct
					{
						std::int32_t value;
						fstring return_value;
					} params;

					params.value = value;

					classes::string->process_event(function, &params);

					return params.return_value;
				}

				static inline fstring float_to_string(const float value) {
					uobject* function = find_obj(L"Engine.KismetStringLibrary.Conv_FloatToString");

					if (function == nullptr)
						return {};

					struct
					{
						float value;
						fstring return_value;
					} params;

					params.value = value;

					classes::string->process_event(function, &params);

					return params.return_value;
				}

				static inline fstring rotator_to_string(const frotator& rotator) {
					uobject* function = find_obj(L"Engine.KismetStringLibrary.Conv_RotatorToString");

					if (function == nullptr)
						return {};

					struct
					{
						frotator rotator;
						fstring return_value;
					} params;

					params.rotator = rotator;

					classes::string->process_event(function, &params);

					return params.return_value;
				}

				static inline fname string_to_name(const wchar_t* string) {
					uobject* function = find_obj(L"Engine.KismetStringLibrary.Conv_StringToName");

					if (function == nullptr) return {};


					struct
					{
						fstring string;
						fname return_value;
					} params;

					params.string = string;

					classes::string->process_event(function, &params);

					return params.return_value;
				}

				static inline fstring name_to_string(const fname& name) {
					uobject* function = find_obj(L"Engine.KismetStringLibrary.Conv_NameToString");

					if (function == nullptr)
						return {};

					struct
					{
						fname name;
						fstring return_value;
					} params;

					params.name = name;

					classes::string->process_event(function, &params);

					return params.return_value;
				}

				static inline ftext string_to_text(const wchar_t* string) {
					uobject* function = find_obj(L"Engine.KismetTextLibrary.Conv_StringToText");

					if (function == nullptr)
						return {};

					struct
					{
						fstring string;
						ftext return_value;
					} params;

					params.string = string;

					classes::text->process_event(function, &params);

					return params.return_value;
				}

				static inline fstring text_to_string(ftext string) {
					uobject* function = find_obj(L"Engine.KismetTextLibrary.Conv_TextToString");

					if (function == nullptr)
						return {};

					struct
					{
						ftext string;
						fstring return_value;
					} params;

					params.string = string;

					classes::text->process_event(function, &params);

					return params.return_value;
				}

				static inline bool starts_with(const wchar_t* source, const wchar_t* prefix, const enum_as_byte<search_case>& search_case = string::ignore_case) {
					uobject* function = find_obj(L"Engine.KismetStringLibrary.StartsWith");

					if (function == nullptr)
						return false;

					struct
					{
						fstring source;
						fstring prefix;
						std::uint8_t search_case;
						bool return_value;
					} params;

					params.source = source;
					params.prefix = prefix;
					params.search_case = search_case.get();

					classes::string->process_event(function, &params);

					return params.return_value;
				}

				static inline bool contains(const wchar_t* search_in, const wchar_t* sub_string, bool use_case = false, bool search_from_end = false) {
					uobject* function = find_obj(L"Engine.KismetStringLibrary.Contains");

					if (function == nullptr)
						return false;

					struct
					{
						fstring search_in;
						fstring sub_string;
						bool use_case;
						bool search_from_end;
						bool return_value;
					} params;

					params.search_in = search_in;
					params.sub_string = sub_string;
					params.use_case = use_case;
					params.search_from_end = search_from_end;

					classes::string->process_event(function, &params);

					return params.return_value;
				}
			};

			class material : public uobject {
			public:

			};

			class material_instance_dynamic : public uobject {
			public:
				material* get_base_material() {
					uobject* function = find_obj(L"Engine.MaterialInterface.GetBaseMaterial");

					struct {
						material* return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				void set_vector_parameter(fname name, flinearcolor value) {
					uobject* function = find_obj(L"Engine.MaterialInstanceDynamic.SetVectorParameterValue");

					struct {
						fname name;
						flinearcolor value;
					} params = { name, value };

					this->process_event(function, &params);
				}

				void set_scalar_parameter(fname name, float value) {
					uobject* function = find_obj(L"Engine.MaterialInstanceDynamic.SetScalarParameterValue");

					struct {
						fname name;
						float value;
					} params = { name, value };

					this->process_event(function, &params);
				}

				flinearcolor get_vector_parameter(fname name) {
					uobject* function = find_obj(L"Engine.MaterialInstanceDynamic.K2_GetVectorParameterValue");

					struct
					{
						fname name;
						flinearcolor return_value;
					} params = { name };

					this->process_event(function, &params);

					return params.return_value;
				}

				float get_scalar_parameter(fname name) {
					uobject* function = find_obj(L"Engine.MaterialInstanceDynamic.K2_GetScalarParameterValue");

					struct
					{
						fname name;
						float return_value;
					} params = { name };

					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class utexture : uobject {
			public:
			};

			class equippable_skin_data_asset : public uobject {
			public:
				struct type {
					const wchar_t* search = nullptr;
					const char* config = nullptr;

					std::int32_t index = 0;

					const bool is_valid() const noexcept {
						return this->search != nullptr || this->config != nullptr;
					}
				};

				static inline equippable_skin_data_asset::type types[] = {
					{ L"default__basepistol", "classic" },
					{ L"default__sawedoffshotgun", "shorty" },
					{ L"default__autopistol", "frenzy" },
					{ L"default__luger", "ghost" },
					{ L"default__revolver", "sheriff" },

					{ L"default__vector", "stinger" },
					{ L"default__mp5", "spectre" }, // 6

					{ L"default__pumpshotgun", "bucky" },
					{ L"default__automaticshotgun", "judge" },

					{ L"default__burst", "bulldog" },
					{ L"default__dmr", "guardian" },
					{ L"default__assaultrifle_acr", "phantom" }, // 11
					{ L"default__ak", "vandal" },

					{ L"default__leversniperrifle", "marshal" },
					{ L"default__boltsniper", "operator" },

					{ L"default__lmg", "ares" },
					{ L"default__heavymachinegun", "odin" }, // 16

					{ L"default__melee", "knife" },

					{ L"default__carbine", "phantom", 11 },
					{ L"default__automaticpistol", "frenzy", 2 },
					{ L"default__subMachinegun_mp5", "spectre", 6 },
					{ L"Default__Slim", "shorty", 1 },
					{ L"Default__hmg", "odin", 16 },
					{ L"Default__lightmachinegun", "ares", 15 },
					{ L"Default__assaultrifle_ak", "vandal", 12 },
					{ L"Default__AssaultRifle_Burst", "bulldog", 9 },
					{ L"Default__AutoShotgun", "judge", 8 },
					{ L"Default__LeverSniper", "marshal", 13 },
				};

				equippable_skin_data_asset::type get_type() {
					const fstring& name = system::get_object_name(this);

					for (std::int32_t index = 0; index < sizeof(equippable_skin_data_asset::types) / sizeof(equippable_skin_data_asset::type); index++) {
						equippable_skin_data_asset::type type = equippable_skin_data_asset::types[index];

						if (type.index == 0)
							type.index = index;

						if (string::contains(name.c_str(), type.search))
							return { type.search, type.config, type.index };
					}

					return {};
				}

				array<int> get_skin_levels()
				{
					return read<array<int>>(std::uintptr_t(this) + 0x88);
				}

			};

			class equippable_skin_chroma_inventory_model : public uobject {
			public:
				uobject* get_skin_chroma_data_asset()
				{
					return read<uobject*>(std::uintptr_t(this) + 0x108);
				}
			};

			class equippable_skin_inventory_model : public uobject {
			public:
				equippable_skin_data_asset* get_skin_data_asset()
				{
					return read<equippable_skin_data_asset*>(std::uintptr_t(this) + 0x108);
				}

				equippable_skin_chroma_inventory_model* get_skin_inventory_chroma_asset()
				{
					uobject* function = find_obj(L"ShooterGame.EquippableSkinInventoryModel.GetEquippedChroma");

					if (function == nullptr)
						return nullptr;

					struct {
						equippable_skin_chroma_inventory_model* return_value;
					} params;


					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class equippable_charm_level_data_asset : public uobject {
			public:
				int get_charm_level()
				{
					return read<int>(std::uintptr_t(this) + 0xe0);
				}
			};

			class charm_data_asset : public uobject {
			public:
				uobject* get_charm_level(int level_number) {
					uobject* function = find_obj(L"ShooterGame.EquippableCharmDataAsset.GetCharmLevel");

					struct {
						int level_number;
						uobject* return_value;
					} params = { level_number };

					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class equippable_charm_instance_inventory_model : public uobject {
			public:
				charm_data_asset* get_charm_data_asset() {
					return read<charm_data_asset*>(std::uintptr_t(this) + 0x68);
				}

				equippable_charm_level_data_asset* get_charm_level_data_asset() {
					return read<equippable_charm_level_data_asset*>(std::uintptr_t(this) + 0x70);
				}
			};

			class equippable_inventory_model : public uobject {
			public:
				int get_is_owned() {
					return read<bool>(std::uintptr_t(this) + 0xfe);
				}

				void set_is_owned(bool status) {
					write<bool>(std::uintptr_t(this) + 0xfe, status);
				}

				equippable_skin_inventory_model* get_equipped_skin_model()
				{
					uobject* function = find_obj(L"ShooterGame.EquippableInventoryModel.GetEquippedSkin");

					if (function == nullptr)
						return nullptr;

					struct {
						equippable_skin_inventory_model* return_value;
					} params;


					this->process_event(function, &params);

					return params.return_value;
				}

				equippable_charm_instance_inventory_model* get_equipped_charm_instance()
				{
					uobject* function = find_obj(L"ShooterGame.EquippableInventoryModel.GetEquippedCharmInstance");

					if (function == nullptr)
						return nullptr;

					struct {
						equippable_charm_instance_inventory_model* return_value;
					} params;


					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class arsenal_view_model : public uobject {
			public:
				array<equippable_inventory_model*> get_gun_models() {
					uobject* function = find_obj(L"ShooterGame.ArsenalViewModel.GetGunModels");

					if (function == nullptr)
						return array<equippable_inventory_model*>();

					struct {
						array<equippable_inventory_model*> return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class arsenal_view_controller : public uobject {
			public:
				arsenal_view_model* get_view_model() {
					uobject* function = find_obj(L"ShooterGame.ArsenalViewController.GetViewModel");

					if (function == nullptr)
						return nullptr;

					struct {
						arsenal_view_model* return_value;
					} params;


					this->process_event(function, &params);

					return params.return_value;
				}

				void equip_buddy(uobject* equippable, uobject* charm_inventory_model) {
					uobject* function = find_obj(L"ShooterGame.ArsenalViewController.EquipBuddy");

					struct {
						uobject* equippable;
						uobject* charm_inventory_model;
					} params = { equippable, charm_inventory_model };

					this->process_event(function, &params);
				}

				void unequip_buddy(uobject* equippable) {
					uobject* function = find_obj(L"ShooterGame.ArsenalViewController.UnequipBuddy");

					this->process_event(function, &equippable);
				}

				void set_skin_with_level_and_chroma(uobject* equippable, uobject* skin_data, uobject* skin_level_data, uobject* chroma_data) {
					uobject* function = find_obj(L"ShooterGame.ArsenalViewController.SetSkinWithLevelAndChromaForGun");

					if (function == nullptr)
						return;

					struct {
						uobject* equippable;
						uobject* skin_data;
						uobject* skin_level_data;
						uobject* chroma_data;
					} params = { equippable, skin_data, skin_level_data, chroma_data };

					this->process_event(function, &params);
				}

				void set_skin_with_chroma(uobject* equippable, uobject* skin_data, uobject* chroma_data) {
					uobject* function = find_obj(L"ShooterGame.ArsenalViewController.SetSkinAndChromaForGun");

					if (function == nullptr)
						return;

					struct {
						uobject* equippable;
						uobject* skin_data;
						uobject* chroma_data;
					} params = { equippable, skin_data, chroma_data };

					this->process_event(function, &params);
				}

				array<equippable_inventory_model*> get_gun_models() {
					return read<array<equippable_inventory_model*>>(std::uintptr_t(this) + 0x98);
				}
			};

			class matchmaking_manager : public uobject {
			public:
				void set_queue_id(fstring queue_id)
				{
					uobject* function = find_obj(L"ShooterGame.MatchmakingManager.SetSelectedQueueID");

					if (function == nullptr)
						return;

					struct {
						fstring q;
					} params = { queue_id };

					this->process_event(function, &params);
				}
			};

			class party_view_controller : public uobject {
			public:
				matchmaking_manager* get_matchmaking_manager()
				{
					return read<matchmaking_manager*>(std::uintptr_t(this) + 0xe8);
				}

				void leave_matchmaking()
				{
					uobject* function = find_obj(L"ShooterGame.PartyViewController.LeaveMatchmaking");

					if (function == nullptr)
						return;

					this->process_event(function, nullptr);
				}

				void leave_party()
				{
					uobject* function = find_obj(L"ShooterGame.PartyViewController.LeaveParty");

					if (function == nullptr)
						return;

					this->process_event(function, nullptr);
				}
			};

			class threaded_chat_manager : public uobject {
			public:
				enum class chat_type : std::uint8_t {
					party, pregame, all, team, ingamesystem, system, whisper, count, max
				};

				void send_chat_message(chat_type type, ftext message)
				{
					uobject* function = find_obj(L"ShooterGame.ThreadedChatManager.SendChatMessageV2");

					if (function == nullptr)
						return;

					struct {
						chat_type type;
						ftext msc;
					} params;

					params.type = type;
					params.msc = message,

						this->process_event(function, &params);
				}
			};

			class juice_inventory_model : public uobject {
			public:
			};

			class base_inventory_model : public uobject {
			public:

			};

			class inventory_manager : public uobject {
			public:
				array<equippable_inventory_model*> get_equippable_models() {
					return read<array<equippable_inventory_model*>>(std::uintptr_t(this) + 0xe0);
				}

				array<equippable_inventory_model*> get_juice_box_models() {
					return read<array<equippable_inventory_model*>>(std::uintptr_t(this) + 0x560);
				}

				array<uobject*> get_all_equippable_charm_instances() {
					uobject* function = find_obj(L"ShooterGame.InventoryManager.GetAllEquippableCharmInstances");

					array<uobject*> return_value;

					this->process_event(function, &return_value);

					return return_value;
				}

				array<uobject*> get_equippable_charm_instances(uobject* charm_data) {
					uobject* function = find_obj(L"ShooterGame.InventoryManager.GetEquippableCharmInstances");

					struct {
						uobject* charm_data;
						array<uobject*> return_value;
					} params = { charm_data };

					this->process_event(function, &params);

					return params.return_value;
				}

				uobject* get_equippable_charm_model(uobject* charm_data) {

					uobject* function = find_obj(L"ShooterGame.InventoryManager.GetEquippableCharmModel");

					struct {
						uobject* charm_data;
						uobject* return_value;
					} params = { charm_data };

					this->process_event(function, &params);

					return params.return_value;
				}

				equippable_skin_inventory_model* get_equippable_skin_model(uobject* skinData)
				{
					uobject* function = find_obj(L"ShooterGame.InventoryManager.GetEquippableSkinModel");

					if (function == nullptr)
						return {};

					struct
					{
						uobject* skindata;
						equippable_skin_inventory_model* return_value;
					} params;

					params.skindata = skinData;

					this->process_event(function, &params);

					return params.return_value;
				}

				equippable_skin_chroma_inventory_model* get_equippable_skin_chroma_model(uobject* chromaData)
				{
					uobject* function = find_obj(L"ShooterGame.InventoryManager.GetEquippableSkinChromaModel");

					if (function == nullptr)
						return {};

					struct
					{
						uobject* skindata;
						equippable_skin_chroma_inventory_model* return_value;
					} params;

					params.skindata = chromaData;

					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class ares_client_game_instace : public uobject {
			public:
				threaded_chat_manager* get_threaded_chat_manager()
				{
					uobject* function = find_obj(L"ShooterGame.AresClientGameInstance.GetThreadedChatManager");

					if (function == nullptr)
						return nullptr;

					struct {
						threaded_chat_manager* return_value;
					} params;


					this->process_event(function, &params);

					return params.return_value;
				}

				party_view_controller* get_party_view_controller()
				{
					uobject* function = find_obj(L"ShooterGame.AresClientGameInstance.GetPartyViewController");

					if (function == nullptr)
						return nullptr;

					struct {
						party_view_controller* return_value;
					} params;


					this->process_event(function, &params);

					return params.return_value;
				}

				arsenal_view_controller* get_aresnal_view_controller()
				{
					uobject* function = find_obj(L"ShooterGame.AresClientGameInstance.GetArsenalViewController");

					if (function == nullptr)
						return nullptr;

					struct {
						arsenal_view_controller* return_value;
					} params;


					this->process_event(function, &params);

					return params.return_value;
				}

				void disconnect_from_server()
				{
					uobject* function = find_obj(L"ShooterGame.AresClientGameInstance.DisconnectFromServer");

					if (function == nullptr)
					{
						////printf(xorstr_("error function\n");
						return;
					}

					this->process_event(function, nullptr);
				}

				void set_display_name(fstring name)
				{
					uobject* function = find_obj(L"ShooterGame.AresClientGameInstance.SetDisplayName");

					if (function == nullptr)
					{
						////printf(xorstr_("error function\n");
						return;
					}

					struct {
						fstring name;
					} params;

					params.name = name;

					this->process_event(function, &params);
				}

				void trigger_vanguard_ban_notification(fstring text)
				{

					uobject* function = find_obj(L"ShooterGame.AresClientGameInstance.TriggerVanguardBanNotification");

					if (function == nullptr)
					{
						////printf(xorstr_("error function\n");
						return;
					}

					struct {
						fstring text;
					} params;

					params.text = text;

					this->process_event(function, &params);
				}

				void request_system_restart(ftext text)
				{

					uobject* function = find_obj(L"ShooterGame.AresClientGameInstance.RequestSystemRestart");

					if (function == nullptr)
					{
						////printf(xorstr_("error function\n");
						return;
					}

					struct {
						ftext text;
					} params;

					params.text = text;

					this->process_event(function, &params);
				}

				inventory_manager* get_inventory_manager() {
					uobject* function = find_obj(L"ShooterGame.AresClientGameInstance.GetInventoryManager");

					inventory_manager* result;

					this->process_event(function, &result);

					return result;
				}
			};

			class ares_instance {
			public:
				static inline class ares_client_game_instace* get_ares_client_game_instance(uobject* world_context)
				{
					uobject* function = find_obj(L"ShooterGame.AresClientGameInstance.GetAresClientGameInstance");

					if (function == nullptr)
					{
						////printf(xorstr_("Errorr\n");
						return {};
					}

					struct {
						uobject* uworld;
						ares_client_game_instace* return_value;
					} params;

					params.uworld = world_context;

					classes::ares_instance->process_event(function, &params);

					return params.return_value;
				}
			};

			class content_library {
			public:
				static void apply_skin(uobject* parent, uobject* skin_asset, uobject* chroma_asset, int skin_level, uobject* charm_asset, int charm_level) {
					uobject* function = find_obj(L"ShooterGame.ContentLibrary.ApplySkin");

					struct {
						uobject* parent;
						uobject* skin_asset;
						uobject* chroma_asset;
						int skin_level;
						uobject* charm_asset;
						int charm_level;
					} params = { parent, skin_asset, chroma_asset, skin_level, charm_asset, charm_level };

					classes::content_library->process_event(function, &params);
				}

				static void clear_weapon_components(uobject* parent) {
					uobject* function = find_obj(L"ShooterGame.ContentLibrary.ClearWeaponComponents");

					classes::content_library->process_event(function, &parent);
				}

				static void add_weapon_attachment(uobject* parent, uintptr_t pointer) {
					uobject* function = find_obj(L"ShooterGame.ContentLibrary.AddWeaponAttachment");

					struct {
						uobject* parent;
						uintptr_t pointer;
					} params = { parent, pointer };

					classes::content_library->process_event(function, &params);
				}
			};

			class math {
			public:
				static inline frotator rotator_interp_to_constant(const frotator& current, const frotator& target, float delta_time, float speed) {
					uobject* function = find_obj(L"Engine.KismetMathLibrary.RInterpTo_Constant");

					struct {
						frotator current;
						frotator target;
						float delta_time;
						float speed;
						frotator return_value;
					} params = { current, target, delta_time, speed };

					classes::math->process_event(function, &params);

					return params.return_value;
				}

				static inline frotator rotator_interp_to(const frotator& current, const frotator& target, float delta_time, float speed) {
					uobject* function = find_obj(L"Engine.KismetMathLibrary.RInterpTo");

					struct {
						frotator current;
						frotator target;
						float delta_time;
						float speed;
						frotator return_value;
					} params = { current, target, delta_time, speed };

					classes::math->process_event(function, &params);

					return params.return_value;
				}

				static inline bool class_is_child_of(uobject* test_class, uobject* parent_class)
				{
					uobject* function = find_obj(L"Engine.KismetMathLibrary.ClassIsChildOf");

					struct {
						uobject* test_class;
						uobject* parent_class;
						bool return_value;
					} params = { test_class, parent_class };

					classes::math->process_event(function, &params);

					return params.return_value;
				}

				static inline fvector mirror_vector_by_normal(fvector in_vect, fvector in_normal)
				{
					uobject* function = find_obj(L"Engine.KismetMathLibrary.MirrorVectorByNormal");

					struct {
						fvector in_vect;
						fvector in_normal;
						fvector return_value;
					} params = { in_vect, in_normal };

					classes::math->process_event(function, &params);

					return params.return_value;
				}

				static inline float vector_size(const fvector& value) {
					uobject* function = find_obj(L"Engine.KismetMathLibrary.VSize");

					struct {
						fvector value;
						float return_value;
					} params = { value };

					classes::math->process_event(function, &params);

					return params.return_value;
				}

				static inline frotator find_look_at_rotation(const fvector& start, const fvector& end) {
					uobject* function = find_obj(L"Engine.KismetMathLibrary.FindLookAtRotation");


					struct {
						fvector start;
						fvector end;
						frotator return_value;
					} params = { start, end };

					classes::math->process_event(function, &params);

					return params.return_value;
				}

				static inline frotator normalized_delta_rotator(const frotator& a, const frotator& b) {
					uobject* function = find_obj(L"Engine.KismetMathLibrary.NormalizedDeltaRotator");

					struct {
						frotator a;
						frotator b;
						frotator return_value;
					} params = { a, b };

					classes::math->process_event(function, &params);

					return params.return_value;
				}
			};

			class test_hud : public uobject {
			public:
				void show_client_error_message(fstring message, float duration) {
					uobject* function = find_obj(L"ShooterGame.ShooterHUD.ShowClientErrorMessage");

					struct {
						fstring message;
						float duration;
					} params = { message, duration };

					this->process_event(function, &params);
				}
			};

			class blueprint {
			public:
				enum class ares_alliance : std::uint8_t {
					ally, enemy, neutral, any, count, max
				};

				static inline fstring get_version() {
					auto function = find_obj(L"ShooterGame.ShooterBlueprintLibrary.GetFullBuildVersionString");

					if (function == nullptr)
						return nullptr;

					fstring return_value;

					classes::blueprint->process_event(function, &return_value);

					return return_value;
				}

				static inline void set_team_fresnel_on_mesh(uobject* meshcomp, uobject* owning_actor, uobject* world_context, array<uobject*> dynamic_materials) {
					// 	void SetTeamFresnelOnMeshComp(struct UMeshComponent* OnMeshComp, struct AActor* OwningActor, struct UObject* __WorldContext, struct TArray<struct UMaterialInstanceDynamic*>& DynamicMaterials); // Function Ares_Characters_FunctionLibrary.Ares_Characters_FunctionLibrary_C.SetTeamFresnelOnMeshComp // (Static|Public|HasOutParms|HasDefaults|BlueprintCallable|BlueprintEvent) // @ game+0x3e32cd0
					auto function = uobject::find_object(L"Ares_Characters_FunctionLibrary.Ares_Characters_FunctionLibrary_C.SetTeamFresnelOnMeshComp", reinterpret_cast<uobject*>(-1));

					struct {
						uobject* mesh;
						uobject* owning_actor;
						uobject* world_context;
						array<uobject*> dynamic_materials;
					} params = { meshcomp, owning_actor, world_context, dynamic_materials };


					classes::blueprint->process_event(function, &params);
				}

				static inline uobject* get_shooter_hud(uobject* world_context) {
					auto function = find_obj(L"ShooterGame.ShooterBlueprintLibrary.GetShooterHUD");

					struct {
						uobject* world_context;
						uobject* return_value;
					} params = { world_context };

					classes::blueprint->process_event(function, &params);

					return params.return_value;
				}

				static inline uobject* spawn_game_object(uobject* world_context, uobject* game_object_class, FTransform transform, uobject* instigated_by, uobject* owner) {
					auto function = find_obj(L"ShooterGame.ShooterBlueprintLibrary.SpawnGameObject");

					struct {
						uobject* world_context;
						uobject* game_object_class;
						FTransform transform;
						uobject* instigated_by;
						uobject* owner;
						uobject* return_value;
					} params = { world_context, game_object_class, transform, instigated_by, owner };

					classes::blueprint->process_event(function, &params);

					return params.return_value;
				}

				static inline class test_hud* get_shooter_hud_test(uobject* world_context) {
					uobject* function = find_obj(L"ShooterGame.ShooterBlueprintLibrary.GetShooterHUD");

					struct {
						uobject* world_context;
						class test_hud* return_value;
					} params = { world_context };

					classes::blueprint->process_event(function, &params);

					return params.return_value;
				}

				template<typename type> static inline array<type*> find_all_shooter_characters_with_alliance(uobject* world_context, uobject* viewer, ares_alliance alliance = ares_alliance::enemy, bool only_player_controlled = false, bool only_alive_players = true) {
					uobject* function = find_obj(L"ShooterGame.ShooterBlueprintLibrary.FindAllShooterCharactersWithAlliance");

					struct {
						uobject* world_context;
						uobject* viewer;
						ares_alliance alliance;
						bool only_player_connected;
						bool only_alive_players;
						array<type*> actors;
					} params = { world_context, viewer, alliance, only_player_controlled, only_alive_players };

					classes::blueprint->process_event(function, &params);

					return params.actors;
				}

				static inline class controller* get_first_local_player_controller(uobject* world_context) {
					uobject* function = find_obj(L"ShooterGame.ShooterBlueprintLibrary.GetFirstLocalPlayerController");

					struct {
						uobject* world_context;
						class controller* return_value;
					} params = { world_context };

					classes::blueprint->process_event(function, &params);

					return params.return_value;
				}

				static inline class controller* get_local_controller(uobject* world_context) {
					uobject* function = find_obj(L"ShooterGame.ShooterBlueprintLibrary.GetLocalController");

					struct {
						uobject* world_context;
						class controller* return_value;
					} params = { world_context };

					classes::blueprint->process_event(function, &params);

					return params.return_value;
				}
			};

			class player_state : public uobject {
			public:
				fstring get_player_name() {
					fstring return_var;

					uobject* function = find_obj(L"Engine.PlayerState.GetPlayerName");

					this->process_event(function, &return_var);

					return return_var;
				}

			};

			class outline_rendering : public uobject {
			public:
				static inline void set_outline_colors_for_render(uobject* world_context, const flinearcolor& ally, const flinearcolor& enemy) {
					uobject* function = find_obj(L"Renderer.AresOutlineRendering.SetOutlineColorsForRender");

					struct {
						uobject* world_context;
						flinearcolor ally;
						flinearcolor enemy;
					} params = { world_context, ally, enemy };

					classes::outline_rendering->process_event(function, &params);
				}
			};

			class material_interface : public uobject {
			public:
				material* get_base_material() {
					uobject* function = find_obj(L"Engine.MaterialInterface.GetBaseMaterial");

					struct {
						material* return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class mesh : public uobject {
			public:
				void set_material(int index, uobject* mat_interface) {
					auto function = find_obj(L"Engine.PrimitiveComponent.SetMaterial");

					struct {
						int index;
						uobject* mat_interface;
					} params = { index, mat_interface };

					this->process_event(function, &params);
				}

				array<material_interface*> get_materials() {
					auto function = find_obj(L"Engine.MeshComponent.GetMaterials");

					struct {
						array<material_interface*> return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				fvector get_forward_vector() {
					auto function = find_obj(L"Engine.SceneComponent.GetForwardVector");

					struct {
						fvector return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				fstring get_bone_name(int bone_index) {
					uobject* function = find_obj(L"Engine.SkinnedMeshComponent.GetBoneName");

					struct {
						int bone_index;
						fname return_value;
					} params = { bone_index };

					this->process_event(function, &params);

					return string::name_to_string(params.return_value);
				}

				void set_outline_mode(outline_mode mode) {
					static void(__fastcall * set_ares_outline)(mesh*, int, int); // vielleicht rauchts ab xD
					if (!set_ares_outline) {
						set_ares_outline = reinterpret_cast<decltype(set_ares_outline)>(memory::get_base_address() + DecryptOffset(offsets::main::set_ares_outline_mode));
					}
					return spoof_call(set_ares_outline)(this, (int)mode, true);
				}

				std::int32_t get_num_bones() {
					uobject* function = find_obj(L"Engine.SkinnedMeshComponent.GetNumBones");

					std::int32_t return_value = 0;
					this->process_event(function, &return_value);

					return return_value;
				}

				fvector bone(std::int32_t index) {
					fmatrix matrix = {};

					static fmatrix* (__fastcall * bone_matrix_func)(mesh*, fmatrix*, std::int32_t);
					if (!bone_matrix_func) {
						bone_matrix_func = reinterpret_cast<decltype(bone_matrix_func)>(memory::get_base_address() + DecryptOffset(offsets::main::bone_matrix));
					}
					spoof_call(bone_matrix_func)(this, &matrix, index);

					return { matrix.w.x, matrix.w.y, matrix.w.z };
				}
			};


			class team_component : public uobject {
			public:
				static inline bool is_ally(uobject* source, uobject* target) {
					uobject* function = find_obj(L"ShooterGame.BaseTeamComponent.IsAlly");

					struct {
						uobject* source;
						uobject* target;
						bool return_value;
					} params = { source, target };

					classes::team_component->process_event(function, &params);

					return params.return_value;
				}

				static inline team_component* get_team_component(uobject* actor) {
					uobject* function = find_obj(L"ShooterGame.BaseTeamComponent.GetTeamComponent");

					struct {
						uobject* source;
						team_component* return_value;
					} params = { actor };

					classes::team_component->process_event(function, &params);

					return params.return_value;
				}
			};

			class stability_component : public uobject {
			public:
				float get_firing_error() {
					uobject* function = find_obj(L"ShooterGame.StabilityComponent.GetFiringError");

					if (function == nullptr)
						return 0.f;

					struct
					{
						float return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				float get_total_error() {
					uobject* function = find_obj(L"ShooterGame.StabilityComponent.GetTotalError");

					if (function == nullptr)
						return 0.f;

					struct
					{
						float return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				float get_movement_error()
				{
					uobject* function = find_obj(L"ShooterGame.StabilityComponent.GetMovementError");

					if (function == nullptr)
						return 0.f;

					struct
					{
						float return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				enum class stability_type : std::uint8_t {
					error, pitch_reocil, yaw_recoil, count, max
				};

				void update_stability_multiplier(stability_type type, float new_multiplier)
				{
					uobject* function = find_obj(L"ShooterGame.StabilityComponent.UpdateStabilityMultiplier");

					if (function == nullptr)
						return;

					struct
					{
						stability_type type;
						float new_multiplier;
					} params = { type, new_multiplier };

					this->process_event(function, &params);
				}


			};

			class fire_state : public uobject {
			public:

				void* get_stability_component() {
					return read<void*>(std::uintptr_t(this) + 0x410);
				}

				void* get_networked_random_number_generator_component() {
					return read<void*>(std::uintptr_t(this) + 0x420);
				}

				float get_error_power() {
					return read<float>(std::uintptr_t(this) + 0x3f0);
				}

				std::int32_t get_error_retries() {
					return read<std::int32_t>(std::uintptr_t(this) + 0x3f4);
				}

			};

			class equippable : public uobject {
			public:
				utexture* get_icon() {
					auto function = find_obj(L"ShooterGame.AresItem.GetItemIcon");

					utexture* return_value;

					this->process_event(function, &return_value);

					return return_value;
				}

				fire_state* get_firing_state_component()
				{
					return read<fire_state*>(std::uintptr_t(this) + 0xff0);
				}

				struct type {
					const wchar_t* search = nullptr;
					const char* config = nullptr;

					std::int32_t index = 0;

					const bool is_valid() const noexcept {
						return this->search != nullptr || this->config != nullptr;
					}
				};

				static inline equippable::type types[] = {
					{ L"basepistol", "classic" },
					{ L"sawedoffshotgun", "shorty" },
					{ L"automaticpistol", "frenzy" },
					{ L"lugerpistol", "ghost" },
					{ L"revolverpistol", "sheriff" },

					{ L"vector", "stinger" },
					{ L"submachinegun_mp5", "spectre" },

					{ L"pumpshotgun", "bucky" },
					{ L"automaticshotgun", "judge" },

					{ L"assaultrifle_burst", "bulldog" },
					{ L"dmr", "guardian" },
					{ L"assaultrifle_acr", "phantom" },
					{ L"assaultrifle_ak", "vandal" },

					{ L"leversniperrifle", "marshal" },
					{ L"boltsniper", "operator" },

					{ L"lightmachinegun", "ares" },
					{ L"heavymachinegun", "odin" },

					{ L"ability_melee_base", "knife" },

					{ L"gun_sprinter_x_heavylightninggun_production", "neon_x" },
					{ L"ability_wushu_x_dagger_production", "jett_dagger" }
				};

				equippable::type get_type() {
					const fstring& name = system::get_object_name(this);

					for (std::int32_t index = 0; index < sizeof(equippable::types) / sizeof(equippable::type); index++) {
						equippable::type type = equippable::types[index];

						if (type.index == 0)
							type.index = index;

						if (string::starts_with(name.c_str(), type.search))
							return { type.search, type.config, type.index };
					}

					return {};
				}

				enum class state : std::uint8_t {
					unknown, idle, equipping, inspecting, attacking, firing, reloading, ads_idle, ads_firing, count
				};

				state get_vfx_state() {
					return *reinterpret_cast<state*>(std::uintptr_t(this) + 0xCC0);
				}

				bool can_fire() {
					if (enum class state state = this->get_vfx_state(); state != state::unknown)
						return state == state::idle || state == state::inspecting || state == state::attacking || state == state::firing || state == state::ads_idle || state == state::ads_firing;

					return false;
				}

				mesh* get_equippable_mesh()
				{
					uobject* function = find_obj(L"ShooterGame.AresEquippable.GetEquippableMesh");

					if (function == nullptr)
						return nullptr;

					struct
					{
						mesh* return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				bool should_present_as_usable() {
					uobject* function = find_obj(L"ShooterGame.AresEquippable.ShouldPresentAsUsable");

					if (function == nullptr)
						return false;

					struct
					{
						bool return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				bool is_zoomed() {
					uobject* function = find_obj(L"ShooterGame.AresEquippable.IsZoomed");

					if (function == nullptr)
						return false;

					struct
					{
						bool return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				equippable_skin_data_asset* get_equippable_skin_data_asset()
				{
					uobject* function = find_obj(L"ShooterGame.AresEquippable.GetEquippableSkinDataAsset");

					equippable_skin_data_asset* return_value;

					this->process_event(function, &return_value);

					return return_value;
				}
			};

			class inventory : public uobject {
			public:
				equippable* get_current_equippable() {
					uobject* function = find_obj(L"ShooterGame.AresInventory.GetCurrentEquippable");

					if (function == nullptr)
						return nullptr;

					struct
					{
						equippable* return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class character_minimap_component : public uobject {
			public:
				void set_observer_status(bool status) {
					if (read<bool>(std::uintptr_t(this) + offsets::minimap_component::observer_status) != status)
						write<bool>(std::uintptr_t(this) + offsets::minimap_component::observer_status, status);
				}

				void set_visible_status(bool status) {
					if (read<bool>(std::uintptr_t(this) + offsets::minimap_component::visible_status) != status)
						write<bool>(std::uintptr_t(this) + offsets::minimap_component::visible_status, status);
				}
			};

			class actor : public uobject {
			public:
				utexture* get_character_icon() {
					auto function = find_obj(L"ShooterGame.ShooterCharacter.GetCharacterIcon");

					utexture* return_value;

					this->process_event(function, &return_value);

					return return_value;
				}

				bool get_firing_location_and_direction(fvector* location, fvector* direction) {
					auto function = find_obj(L"ShooterGame.ShooterCharacter.GetFiringLocationAndDirection");

					struct {
						fvector location;
						fvector direction;
						bool return_value;
					} params;

					this->process_event(function, &params);

					if (location)
						*location = params.location;

					if (direction)
						*direction = params.direction;

					return params.return_value;
				}


				character_minimap_component* get_portrait_minimap_component() {
					return read<character_minimap_component*>(std::uintptr_t(this) + offsets::radar::portrait_minimap);
				}

				character_minimap_component* get_character_minimap_component() {
					return read<character_minimap_component*>(std::uintptr_t(this) + offsets::radar::character_minimap);
				}

				void add_controller_pitch_input(float value) { // Possible Dtc
					uobject* function = find_obj(L"Engine.Pawn.AddControllerPitchInput");

					this->process_event(function, &value);
				}

				void add_controller_yaw_input(float value) { // Possible Dtc
					uobject* function = find_obj(L"Engine.Pawn.AddControllerYawInput");

					this->process_event(function, &value);
				}

				actor* simple_trace_from_camera(float distance, collision_channel channel, fvector* hit_point, bool* blocking_hit) {
					uobject* function = find_obj(L"ShooterGame.ShooterCharacter.SimpleTraceFromCamera");

					struct {
						float distance;
						collision_channel channel;
						fvector hit_point;
						bool blocking_hit;
						actor* return_value;
					} params = { distance, channel };

					this->process_event(function, &params);

					if (hit_point != nullptr) {
						*hit_point = params.hit_point;
					}

					if (blocking_hit != nullptr) {
						*blocking_hit = params.blocking_hit;
					}

					return params.return_value;
				}

				void disable_input(controller* value) {
					uobject* function = find_obj(L"Engine.Actor.DisableInput");
					this->process_event(function, &value);
				}

				void enable_input(controller* value) {
					uobject* function = find_obj(L"Engine.Actor.EnableInput");
					this->process_event(function, &value);
				}

				void set_3p_mesh_visible(bool state) {
					uobject* function = find_obj(L"ShooterGame.ShooterCharacter.Set3PMeshVisible");
					this->process_event(function, &state);
				}

				frotator get_view_rotation_with_full_recoil() {
					frotator return_value;

					uobject* function = find_obj(L"ShooterGame.ShooterCharacter.GetViewRotationFullRecoil");
					this->process_event(function, &return_value);

					return return_value;
				}

				frotator get_view_rotation_with_no_recoil() {
					frotator return_value;

					uobject* function = find_obj(L"ShooterGame.ShooterCharacter.GetViewRotationNoRecoil");

					this->process_event(function, &return_value);

					return return_value;
				}

				inventory* get_inventory() {

					uobject* function = find_obj(L"ShooterGame.ShooterCharacter.GetInventory");

					inventory* return_value;
					this->process_event(function, &return_value);

					return return_value;
				}

				bool is_still_firing() {
					uobject* function = find_obj(L"ShooterGame.ShooterCharacter.IsStillFiring");

					if (function == nullptr)
						return false;

					bool return_value;
					this->process_event(function, &return_value);

					return return_value;
				}

				bool is_dormant() {
					return (read<bool>(std::uintptr_t(this) + offsets::actor::dormant) == 1);
				}

				bool is_alive() {
					uobject* function = find_obj(L"ShooterGame.ShooterCharacter.IsAlive");

					bool return_value;
					this->process_event(function, &return_value);

					return return_value;
				}

				bool can_jump() {
					uobject* function = find_obj(L"Engine.Character.CanJump");

					bool return_value;
					this->process_event(function, &return_value);

					return return_value;
				}

				mesh* get_mesh() {
					uobject* function = find_obj(L"ShooterGame.ShooterCharacter.GetPawnMesh");

					mesh* return_value;
					this->process_event(function, &return_value);

					return return_value;
				}

				bool is_player_character() {
					uobject* function = find_obj(L"ShooterGame.ShooterCharacter.IsPlayerCharacter");

					bool return_value;
					this->process_event(function, &return_value);

					return return_value;
				}

				fvector get_location() {
					auto function = find_obj(L"Engine.Actor.K2_GetActorLocation");

					fvector return_value;
					this->process_event(function, &return_value);

					return return_value;
				}

				frotator get_rotation() {
					auto function = find_obj(L"Engine.Actor.K2_GetActorRotation");

					frotator return_value;
					this->process_event(function, &return_value);

					return return_value;
				}

				float get_health() {
					auto function = find_obj(L"ShooterGame.ShooterCharacter.GetHealth");

					float return_value;
					this->process_event(function, &return_value);

					if (return_value < 0) return_value = 0.0f;
					if (return_value > 100) return_value = 100.0f;

					return return_value;
				}

				float get_shield() {
					auto function = find_obj(L"ShooterGame.ShooterCharacter.GetShield");

					float return_value;
					this->process_event(function, &return_value);

					return return_value;
				}

				player_state* get_player_state() {
					auto function = find_obj(L"ShooterGame.ShooterCharacter.GetPlayerState");

					if (function == nullptr)
						return nullptr;

					player_state* return_value;
					this->process_event(function, &return_value);

					return return_value;
				}
			};

			class camera : public uobject {
			public:
				fvector get_location() {
					uobject* function = find_obj(L"Engine.PlayerCameraManager.GetCameraLocation");

					fvector return_value;
					this->process_event(function, &return_value);

					return return_value;
				}

				frotator get_rotation() {
					uobject* function = find_obj(L"Engine.PlayerCameraManager.GetCameraRotation");

					if (function == nullptr)
						return {};

					struct
					{
						frotator return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class cheat_manager : public uobject {
			public:
				void unlock_all_agents()
				{
					uobject* function = find_obj(L"ShooterGame.PregameCheatManager.UnlockAllCharacters");

					if (function == nullptr)
					{
						return;
					}

					this->process_event(function, nullptr);
				}

				void set_account_level(int level)
				{
					uobject* function = find_obj(L"ShooterGame.MainMenuCheatManager.SetAccountLevel");

					if (function == nullptr)
						return;

					struct
					{
						int level;
					} params = { level };

					this->process_event(function, &params);
				}
			};

			class pregame_view_model : public uobject {
			public:
				bool is_local_player_locked_in()
				{
					uobject* function = find_obj(L"ShooterGame.PregameViewModel.IsLocalPlayerLockedIn");

					if (function == nullptr)
						return false;

					struct
					{
						bool return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class pregame_view_controller : public uobject {
			public:
				pregame_view_model* get_pregame_view_model() {
					uobject* function = find_obj(L"ShooterGame.PregameViewController.GetViewModel");

					if (function == nullptr)
						return nullptr;

					struct
					{
						pregame_view_model* return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				void lock_character(uobject* agent)
				{
					uobject* function = find_obj(L"ShooterGame.PregameViewController.LockCharacter");

					if (function == nullptr)
						return;

					struct
					{
						uobject* agent;
					} params = { agent };

					this->process_event(function, &params);
				}

				void select_character(uobject* agent)
				{
					uobject* function = find_obj(L"ShooterGame.PregameViewController.SelectCharacter");

					if (function == nullptr)
						return;

					struct
					{
						uobject* agent;
					} params = { agent };

					this->process_event(function, &params);
				}
			};

			class player_input : public uobject {
			public:
				void input_key(fkey key, int event_type, float amount_depressed = 1.f, bool game_pad = false) {
					/*
						IE_Pressed = 0,
						IE_Released = 1,
					*/
					static void(__fastcall * input_key_func)(uobject*, fkey, int, float, bool); // vielleicht rauchts ab xD
					if (!input_key_func) {
						input_key_func = reinterpret_cast<decltype(input_key_func)>(memory::get_base_address() + DecryptOffsetClear(offsets::main::input_key));
					}
					spoof_call(input_key_func)(this, key, event_type, amount_depressed, game_pad);
				}
			};

			class controller : public uobject {
			public:
				void add_pitch(float value) {
					static void(__fastcall * input_key_func)(controller*, float);
					if (!input_key_func) {
						input_key_func = reinterpret_cast<decltype(input_key_func)>(memory::get_base_address() + DecryptOffsetClear(offsets::main::add_pitch));
					}
					spoof_call(input_key_func)(this, value);
				}

				void add_yaw(float value) { // Possible Dtc
					static void(__fastcall * input_key_func)(controller*, float);
					if (!input_key_func) {
						input_key_func = reinterpret_cast<decltype(input_key_func)>(memory::get_base_address() + DecryptOffsetClear(offsets::main::add_yaw));
					}
					spoof_call(input_key_func)(this, value);
				}

				player_input* get_player_input() {
					return read<player_input*>(std::uintptr_t(this) + offsets::controller::player_input);
				}

				void disconnect_from_server()
				{
					uobject* function = find_obj(L"ShooterGame.AresPlayerController.DisconnectFromServer");
					this->process_event(function, nullptr);
				}

				pregame_view_controller* get_pregame_view_controller() {
					return read<pregame_view_controller*>(std::uintptr_t(this) + offsets::controller::pregame_view_controller);
				}

				bool get_mouse_position(float& x, float& y)
				{
					uobject* function = find_obj(L"Engine.PlayerController.GetMousePosition");
					if (function == nullptr)
						return false;

					struct
					{
						float x;
						float y;
						bool success;
					} params;

					this->process_event(function, &params);

					x = params.x;
					y = params.y;

					return params.success;
				}

				camera* get_camera() {
					uobject* function = find_obj(L"ShooterGame.AresPlayerController.GetPlayerCameraManager");

					if (function == nullptr)
						return nullptr;

					struct
					{
						camera* return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				actor* get_pawn() {
					uobject* function = find_obj(L"Engine.Controller.K2_GetPawn");

					struct
					{
						actor* return_value;
					} params;

					this->process_event(function, &params);

					return params.return_value;
				}

				bool is_input_key_down(fkey key) {
					uobject* function = find_obj(L"Engine.PlayerController.IsInputKeyDown");

					struct {
						fkey key;
						bool return_value;
					} params = { key };

					this->process_event(function, &params);

					return params.return_value;
				}

				bool was_input_key_just_pressed(fkey key) {
					uobject* function = find_obj(L"Engine.PlayerController.WasInputKeyJustPressed");

					struct {
						fkey key;
						bool return_value;
					} params = { key };

					this->process_event(function, &params);

					return params.return_value;
				}

				void simulate_input_key(fkey key, bool pressed = true) {
					if (auto input = this->get_player_input()) {
						input->input_key(key, pressed ? 0 : 1);
					}
				}

				frotator get_control_rotation() {
					uobject* function = find_obj(L"Engine.Controller.GetControlRotation");

					frotator return_value = {};

					if (function == nullptr)
						return return_value;

					this->process_event(function, &return_value);

					return return_value;
				}

				float get_mouse_zoom_sensitivity() {
					uobject* function = find_obj(L"ShooterGame.ShooterPlayerController.GetMouseZoomSensitivity");

					float return_value = 0.f;

					if (function == nullptr)
						return return_value;

					this->process_event(function, &return_value);

					return return_value;
				}

				float get_mouse_sensitivity() {
					uobject* function = find_obj(L"ShooterGame.ShooterPlayerController.GetMouseSensitivity");

					float return_value = 0.f;

					if (function == nullptr)
						return return_value;

					this->process_event(function, &return_value);

					return return_value;
				}

				float get_sensitivity(equippable* equippable) {
					float sensitivity = this->get_mouse_sensitivity();

					if (equippable->is_zoomed())
						sensitivity *= this->get_mouse_zoom_sensitivity();

					return sensitivity;
				}
			};

			class projectil_component : public uobject {
			public:
				array<fvector> estimate_projectile_trajectory(uobject* thrower, fvector fire_location, fvector fire_direction, array<uobject*> ignore_actors, int child_actor_ignore_settings, float time_step, float max_estimated_distance, int max_estimated_bounces, float max_estimated_time) {
					auto function = find_obj(L"ShooterGame.ProjectileEstimationComponent.EstimateProjectileTrajectory");

					/*

				RESULT 	struct TArray<struct FVector>


					AShooterCharacter* Thrower,
					FVector& FiringLocation,
					FVector& FiringDirection,
					TArray<struct AActor*>& IgnoredActors,
					enum class EChildActorIgnoreSetting ChildActorIgnoreSetting,
					float TimeStep,
					float MaxEstimatedDistance,
					int32_t MaxEstimatedBounces,
					float MaxEstimatedTime

					*/

					struct {
						uobject* thrower;
						fvector fire_location;
						fvector fire_direction;
						array<uobject*> ignore_actors;
						uint8_t child_actor_ignore_settings;
						float time_step;
						float max_estimated_distance;
						uint32_t max_estimated_bounces;
						float max_estimated_time;
						array<fvector> result_value;
					} params = { thrower, fire_location, fire_direction, ignore_actors, child_actor_ignore_settings, time_step, max_estimated_distance, max_estimated_bounces, max_estimated_time };

					this->process_event(function, &params);

					return params.result_value;
				}
			};

			class finisher_fxc : public uobject {
			public:
				void start_effect(uobject* target, uobject* context, float start_time, bool first_person) {
					auto function = uobject::find_object(L"FXC_Finisher_Atlas_Victim.FXC_Finisher_Atlas_Victim_C.StartEffect", reinterpret_cast<uobject*>(-1));
					if (!function) {
						printf("cant find function\n");
						return;
					}

					struct {
						uobject* target;
						uobject* context;
						float start_time;
						bool first_person;
					} params = { target, context, start_time, first_person };

					this->process_event(function, &params);
				}
			};

			class gameplay_statics {
			public:
				struct predict_projectile_path_params {
					fvector start_location;
					fvector launch_velocity;
					bool trace_with_collision = true;
					char pad_19[0x3];
					float projectile_radius = 10;
					float max_sim_time = 2;
					bool trace_with_channel = true;
					collision_channel trace_channel = collision_channel::ECC_WorldDynamic;
					char pad_26[0x2];
					char adssad[0x10];
					array<actor*> actors_to_ignore;
					float sim_frequency = 40;
					float override_gravity_z = 0;
					debug_trace_channel draw_debug_type = debug_trace_channel::None;
					char pad_51[0x3];
					float draw_debugt_time = 1;
					bool trace_complex = false;
					char pad_59[0x7];
				};

				struct point_data {
					struct fvector location; // 0x00(0x0c)
					struct fvector velocity; // 0x0c(0x0c)
					float time; // 0x18(0x04)
				};

				struct path_result {
					array<point_data> path_data; // 0x00(0x10)
					point_data LastTraceDestination; // 0x10(0x1c)
					fhitresult hit_result; // Hitresult
				};

			public:
				static inline uobject* finish_spawning_actor(uobject* actor, FTransform spawn_transform) {
					auto function = find_obj(L"Engine.GameplayStatics.FinishSpawningActor");

					struct {
						uobject* actor;
						FTransform spawn_transform;
						uobject* return_value;
					} params = { actor, spawn_transform };

					classes::gameplay_statics->process_event(function, &params);

					return params.return_value;
				}

				static inline uobject* begin_spawning_actor(uobject* world_context, uobject* actor_class, FTransform spawn_transform, bool no_collision, uobject* owner) {
					auto function = find_obj(L"Engine.GameplayStatics.BeginSpawningActorFromClass");

					struct {
						uobject* world_context;
						uobject* actor_class;
						FTransform spawn_transform;
						bool no_collision;
						uobject* owner;
						uobject* return_value;
					} params = { world_context, actor_class, spawn_transform, no_collision, owner };

					classes::gameplay_statics->process_event(function, &params);

					return params.return_value;
				}

				static inline uobject* spawn_object(uobject* object_class, uobject* outer) {
					auto function = find_obj(L"Engine.GameplayStatics.SpawnObject");

					struct {
						uobject* object_class;
						uobject* outer;
						uobject* return_value;
					} params = { object_class, outer };

					classes::gameplay_statics->process_event(function, &params);

					return params.return_value;
				}

				static inline uobject* get_actor_of_class(uobject* world_context, uobject* actor_class) {
					auto function = find_obj(L"Engine.GameplayStatics.GetActorOfClass");

					struct {
						uobject* world_context;
						uobject* actor_class;
						uobject* return_value;
					} params = { world_context, actor_class };

					classes::gameplay_statics->process_event(function, &params);

					return params.return_value;
				}

				static inline bool predict_projectile_path(uobject* world_context, fvector start_location, fvector start_direction, array<actor*> actors_to_ignore, float override_graivity_z, path_result* result)
				{
					auto function = find_obj(L"Engine.GameplayStatics.Blueprint_PredictProjectilePath_Advanced");

					gameplay_statics::predict_projectile_path_params predict_params = gameplay_statics::predict_projectile_path_params();
					predict_params.start_location = start_location;
					predict_params.launch_velocity = start_direction;
					predict_params.projectile_radius = 5;
					predict_params.trace_channel = collision_channel::ECC_WorldDynamic;
					predict_params.trace_complex = false;
					predict_params.trace_with_collision = true;
					predict_params.trace_with_channel = true;
					predict_params.actors_to_ignore = actors_to_ignore;
					predict_params.draw_debug_type = debug_trace_channel::None;
					predict_params.draw_debugt_time = 1;
					predict_params.sim_frequency = 40;
					predict_params.max_sim_time = 2;
					predict_params.override_gravity_z = override_graivity_z;

					struct {
						uobject* world_context;
						predict_projectile_path_params predict_params;
						path_result path_result;
						bool return_value;
					} params = { world_context, predict_params };

					classes::gameplay_statics->process_event(function, &params);

					if (result) {
						*result = params.path_result;
					}

					return params.return_value;
				}

				template<typename type>
				static inline array<type*> get_all_actors_of_class(uobject* world_context, uclass* filter) {
					uobject* function = find_obj(L"Engine.GameplayStatics.GetAllActorsOfClass");

					if (function == nullptr || world_context == nullptr || filter == nullptr)
						return {};

					struct
					{
						uobject* world_context;
						uclass* filter;
						array<type*> actors;
					} params;

					params.world_context = world_context;
					params.filter = filter;

					classes::gameplay_statics->process_event(function, &params);

					return params.actors;
				}

				static inline float get_world_delta_seconds(uobject* world_context) {
					uobject* function = find_obj(L"Engine.GameplayStatics.GetWorldDeltaSeconds");

					if (function == nullptr)
						return 0.0f;

					struct {
						uobject* world_context;
						float return_value;
					} params;

					params.world_context = world_context;

					classes::gameplay_statics->process_event(function, &params);

					return params.return_value;
				}

				static inline float get_time_seconds(uobject* world_context) {
					uobject* function = find_obj(L"Engine.GameplayStatics.GetTimeSeconds");

					if (function == nullptr)
						return 0.0f;

					struct {
						uobject* world_context;
						float return_value;
					} params;

					params.world_context = world_context;

					classes::gameplay_statics->process_event(function, &params);

					return params.return_value;
				}

				static inline uobject* get_game_instance(uobject* world_context) {
					uobject* function = find_obj(L"Engine.GameplayStatics.GetGameInstance");

					if (function == nullptr)
						return {};

					struct {
						uobject* world_context;
						uobject* return_value;
					} params;

					params.world_context = world_context;

					classes::gameplay_statics->process_event(function, &params);

					return params.return_value;
				}
			};

			class canvas : public uobject {
			public:
				int getSizeX() {
					return read<int>(std::uintptr_t(this) + offsets::canvas::size_x);
				}

				int getSizeY() {
					return read<int>(std::uintptr_t(this) + offsets::canvas::size_y);
				}

				void draw_box(const fvector2d& position, const fvector2d& size, float thickness, const flinearcolor& color) {
					uobject* function = find_obj(L"Engine.Canvas.K2_DrawBox");

					struct {
						fvector2d position;
						fvector2d size;
						float thickness;
						flinearcolor color;
					} params = { position, size, thickness, color };

					this->process_event(function, &params);
				}

				void draw_texture(utexture* texture, fvector2d screen_position, fvector2d screen_size, fvector2d coordinate_position, fvector2d coordinate_size, flinearcolor render_color, int blend_mode, float rotation, fvector2d pivot_point) {
					uobject* function = find_obj(L"Engine.Canvas.K2_DrawTexture");

					struct {
						utexture* texture;
						fvector2d screen_position;
						fvector2d screen_size;
						fvector2d coordinate_position;
						fvector2d coordinate_size;
						flinearcolor render_color;
						int blend_mode; // 1?
						float rotation;
						fvector2d pivot_point;
					} params = { texture, screen_position, screen_size, coordinate_position, coordinate_size, render_color, blend_mode, rotation, pivot_point };

					this->process_event(function, &params);
				}

				void draw_text(uobject* font, const fstring& text, const fvector2d& position, const fvector2d& scale, const flinearcolor& color, float kerning, const flinearcolor& shadow_color, const fvector2d& shadow_offset, bool centre_x, bool centre_y, bool outlined, const flinearcolor& outline_color) {
					uobject* function = find_obj(L"Engine.Canvas.K2_DrawText");

					struct {
						uobject* font;
						fstring text;
						fvector2d position;
						fvector2d scale;
						flinearcolor color;
						float kerning;
						flinearcolor shadow_color;
						fvector2d shadow_offset;
						bool centre_x;
						bool centre_y;
						bool outlined;
						flinearcolor outline_color;
					} params = { font, text, position, scale, color, kerning, shadow_color, shadow_offset, centre_x, centre_y, outlined, outline_color };

					this->process_event(function, &params);
				}

				void draw_line(const fvector2d screen_position_a, const fvector2d screen_position_b, float thickness, const flinearcolor& render_color) {
					uobject* function = find_obj(L"Engine.Canvas.K2_DrawLine");

					struct {
						fvector2d screenposa;
						fvector2d screenposb;
						float thickness;
						flinearcolor render;
					} params = { screen_position_a, screen_position_b, thickness, render_color };

					this->process_event(function, &params);
				}

				void draw_transparent_line(const fvector2d screen_position_a, const fvector2d screen_position_b, float thickness, const flinearcolor& render_color) {
					uobject* function = find_obj(L"Engine.Canvas.K2_DrawTransparentLine");

					struct {
						fvector2d screenposa;
						fvector2d screenposb;
						float thickness;
						flinearcolor render;
					} params = { screen_position_a, screen_position_b, thickness, render_color };

					this->process_event(function, &params);
				}

				void draw_circle(int x, int y, int radius, int numsides, flinearcolor color) {
					float step = 3.14159265358979323846 * 2.0 / numsides;
					int count = 0;

					fvector2d v[128];

					for (float a = 0; a < 3.14159265358979323846 * 2.0; a += step) {
						float x1 = radius * cosf(a) + x;
						float y1 = radius * sinf(a) + y;
						float x2 = radius * cosf(a + step) + x;
						float y2 = radius * sinf(a + step) + y;

						v[count].x = x1;
						v[count].y = y1;
						v[count + 1].x = x2;
						v[count + 1].y = y2;

						this->draw_line(fvector2d({ v[count].x, v[count].y }), fvector2d({ x2, y2 }), 1.0f, color);
					}
				}

				fvector project(const fvector& world_location) {
					uobject* function = find_obj(L"Engine.Canvas.K2_Project");

					struct {
						fvector world_location;
						fvector return_value;
					} params = { world_location };

					this->process_event(function, &params);

					return params.return_value;
				}

				fvector2d strlen(uobject* font, const fstring text) {
					uobject* function = find_obj(L"Engine.Canvas.K2_StrLen");

					struct {
						uobject* font;
						fstring text;
						fvector2d return_value;
					} params = { font, text };

					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class engine {
			public:

				uobject* get_medium_font() {
					return read<uobject*>(std::uintptr_t(this) + offsets::engine::medium_font);
				}

				uobject* get_subtitle_font() {
					return read<uobject*>(std::uintptr_t(this) + offsets::engine::subtitle_font);
				}
			};

			class game_viewport_client {
			public:
				uobject* get_world() {
					return read<uobject*>(std::uintptr_t(this) + offsets::viewport_client::world);
				}

				uobject* get_game_instance() noexcept {
					return read<uobject*>(std::uintptr_t(this) + offsets::viewport_client::game_instance);
				}

				engine* get_engine() noexcept {
					return reinterpret_cast<engine*>(system::get_outer_object(this->get_game_instance()));
				}
			};

			struct fminimal_view_info {
				fvector Location;
				fvector Rotation;
				float FOV;
			};

			struct fview_target {
				struct AActor* Target;
				char pad_8[0x8];
				fminimal_view_info POV;
				player_state* PlayerState;
				char pad_6A8[0x8];
			};
		}
	}
}

using namespace pinguu::sdk::valorant;