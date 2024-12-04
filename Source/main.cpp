#include "includes.h"


class hook {
public:
	static void initialize() {
		pinguu::vmt::shadow::rwx_cave = pinguu::vmt::shadow::find_whitelisted_rwx_cave();

		hook_draw_transition();
	}

	static void uninitialize() {
		pinguu::vmt::shadow::unbind_all_functions();
	}

	static void hooked_transition(game_viewport_client* instance, canvas* canvas) {
		if (auto worldInstance = instance->get_world()) {
			globals::set_world_instance(worldInstance);
			globals::set_canvas(canvas);

			gui::defines::_canvas = globals::get_canvas();

			// @ init some stuff
			if (!bInit) {
				pinguu::insta_lock_agents::run();
				bInit = true;
			}

			// @ init unlock skins
			if (!bSkinsUnlocked) {
				auto tMenuText = Encrypt(L"MainMenuv2");

				if (string::contains(worldInstance->get_name().c_str(), tMenuText.decrypt())) {
					if (global::skin_changer::config.bEnabled) {
						updater::unlock_all_skins(worldInstance);
						bSkinsUnlocked = true;
					}
				}

				tMenuText.encrypt();
			}

			if (auto playerController = blueprint::get_first_local_player_controller(worldInstance)) { // @ can i get the player_controller from game_viewport_client?
				globals::set_player_controller(playerController);
				pinguu::insta_lock::run(playerController);

				if (auto localPawn = playerController->get_pawn()) {
					globals::set_local_pawn(localPawn);

					globals::get_players().clear();

					array<actor*> actors = blueprint::find_all_shooter_characters_with_alliance<actor>(playerController, playerController, blueprint::ares_alliance::any);
					std::vector<actor*> actor_cache;
					for (int i = 0; i < actors.size(); i++) {
						if (auto actor = actors[i]) {
							if (actor == localPawn) continue;

							actor_cache.push_back(actor);
						}
					}

					globals::set_players(actor_cache);

					if (localPawn != aPawnOld) aPawnOld = localPawn;
					if (playerController != cControllerOld) cControllerOld = playerController;

					pinguu::aimbot::run(playerController);
					pinguu::chams::run(playerController);
					pinguu::misc::run(playerController);
					pinguu::triggerbot::run();
					pinguu::standalone_recoil::run();
					pinguu::esp::run();
					pinguu::skin_changer::run();
				}
			}

			//pinguu::menu::menu_tick();
		}

		if (pinguu::menu::defines::uninject) {
			shadowViewport.restore_table();
			pinguu::vmt::shadow::unbind_all_functions();
		}

		return spoof_call(oDrawTransition)(instance, canvas);
	}

	static void hook_draw_transition() {
		if (uobject* game_instance = gameplay_statics::get_game_instance(world_finder::find_uworld())) {
			if (engine* game_engine = (engine*)system::get_outer_object(game_instance)) {
				if (game_viewport_client* viewport_client = read<game_viewport_client*>(std::uintptr_t(game_engine) + offsets::main::view_port)) {
					shadowViewport = pinguu::vmt::shadow(viewport_client);
					oDrawTransition = shadowViewport.apply<decltype(oDrawTransition)>(pinguu::vmt::shadow::bind_function_to_rwx_cave(hooked_transition), offsets::main::post_render);
				}
			}
		}
	}

private:
	static inline pinguu::vmt::shadow shadowViewport;
	static inline void(__fastcall* oDrawTransition)(game_viewport_client*, canvas*);
	static inline bool bInit = false;
	static inline bool bSkinsUnlocked = false;
	static inline controller* cControllerOld;
	static inline actor* aPawnOld;
};

class imgui_hook {
public:
	static HRESULT present_hooked(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
		if (!Gui::initializeGui(swapChain, syncInterval, flags)) {
			printf("[X] Error Initializing GUI!\n");
			return spoof_call(present_original)(swapChain, syncInterval, flags);
		}

		Gui::inputHook(true);
		Gui::startFrame();

		ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 10), ImGui::GetColorU32({ 1.f, 0.f, 0.f, 1.f }), EncryptString("(DEV. BUILD)"));

		new_menu::draw_menu();

		Gui::endFrame();

		return spoof_call(present_original)(swapChain, syncInterval, flags);
	}

	static HRESULT resize_hooked(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {

		if (Gui::pRenderTargetView) {
			Gui::pContext->OMSetRenderTargets(0, 0, 0);
			Gui::pRenderTargetView->Release();
		}

		HRESULT hr = spoof_call(resize_original)(pThis, BufferCount, Width, Height, NewFormat, SwapChainFlags);

		ID3D11Texture2D* pBuffer;
		pThis->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

		if (pBuffer) {
			Gui::pDevice->CreateRenderTargetView(pBuffer, NULL, &Gui::pRenderTargetView);
			pBuffer->Release();
		}

		Gui::pContext->OMSetRenderTargets(1, &Gui::pRenderTargetView, NULL);

		D3D11_VIEWPORT vp;
		vp.Width = Width;
		vp.Height = Height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		Gui::pContext->RSSetViewports(1, &vp);

		return hr;
	}

	void init() {
		auto enc_window = Encrypt("UnrealWindow");
		auto enc_game = Encrypt("VALORANT  ");

		auto level = D3D_FEATURE_LEVEL_11_0;
		DXGI_SWAP_CHAIN_DESC sd;
		{
			ZeroMemory(&sd, sizeof sd);
			sd.BufferCount = 1;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = IFH(FindWindowA)(enc_window.decrypt(), enc_game.decrypt());
			sd.SampleDesc.Count = 1;
			sd.Windowed = TRUE;
			sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		}

		enc_window.clear();
		enc_game.clear();

		IDXGISwapChain* swap_chain = nullptr;
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;

		spoof_call(D3D11CreateDeviceAndSwapChain)(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &level, 1, D3D11_SDK_VERSION, &sd, &swap_chain, &device, nullptr, &context);

		auto* swap_chainvtable = reinterpret_cast<uintptr_t*>(swap_chain);
		swap_chainvtable = reinterpret_cast<uintptr_t*>(swap_chainvtable[0]);

		DWORD old_protect, old_protect_resize;

		spoof_call(IFH(VirtualProtect))(swap_chainvtable, 0x2B8, PAGE_EXECUTE_READWRITE, &old_protect);
		present_hook = memory_hook::create_context(swap_chainvtable[8]);
		present_original = present_hook->hook(present_hooked);
		present_hook->enable_hook();
		spoof_call(IFH(VirtualProtect))(swap_chainvtable, 0x2B8, old_protect, &old_protect);

		spoof_call(IFH(VirtualProtect))(swap_chainvtable, 0x2B8, PAGE_EXECUTE_READWRITE, &old_protect_resize);
		resize_hook = memory_hook::create_context(swap_chainvtable[13]);
		resize_original = resize_hook->hook(resize_hooked);
		resize_hook->enable_hook();
		spoof_call(IFH(VirtualProtect))(swap_chainvtable, 0x2B8, old_protect_resize, &old_protect_resize);
	}
private:
	memory_hook::memhook_context* present_hook;
	memory_hook::memhook_context* resize_hook;

	typedef HRESULT(*present_fn)(IDXGISwapChain*, UINT, UINT);
	static inline present_fn present_original{ };

	typedef HRESULT(*resize_fn)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
	static inline resize_fn resize_original{ };
};

class main : public globals {
public:
	void run() {
		pinguu::config::load_config();
		
		memory::set_base_address(std::uintptr_t(IFH(GetModuleHandleA)(0)));

		classes::initialize(); // @ Init classes	
		pinguu::keys::run(); // @ Init keys
		hook::initialize(); // @ Init hooks

		if (uobject* game_instance = gameplay_statics::get_game_instance(world_finder::find_uworld())) {
			if (engine* game_engine = (engine*)system::get_outer_object(game_instance)) {
				RobotoBold = (uintptr_t*)game_engine->get_subtitle_font();
				RobotoRegular = (uintptr_t*)game_engine->get_medium_font();
				NamesFont = (uintptr_t*)game_engine->get_subtitle_font();
			}
		}
	}
};

bool __stdcall DllMain(const HMODULE instance, const unsigned long reason, const void* reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		/*AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		freopen_s((FILE**)stdin, "CONIN$", "r", stdin);*/

		imgui_hook().init();
		main().run();

		return true;
	}
	return false;
}

// custom entry point