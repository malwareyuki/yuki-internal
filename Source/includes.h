#pragma once
//#define TESTING

#include <Windows.h>
#include <array>
#include <vector>
#include <string>  
#include <chrono>
#include <D3D11.h>

class memory {
public:
	static inline void set_base_address(uintptr_t base) {
		base_address = base;
	}

	static inline uintptr_t get_base_address() {
		return base_address;
	}

private:
	static inline uintptr_t base_address = 0x0;
};

uintptr_t* NamesFont = NULL;
uintptr_t* RobotoBold = NULL;
uintptr_t* RobotoRegular = NULL;
uintptr_t* OtherNamesFont = NULL;

#include "Core/framework.h"
#include "hooking/memory_hook.h"
#include "spoofing.h"
#include "xor_string.h"
#include "valo_offsets.h"
#include "unreal.h"
#include "base.h"
#include "memory.h"
#include "uworld_update.h"
#include "sdk.h"
#include "pointer.h"
#include "hooking.h"
#include "utils.h"
#include "keys.h"
#include "valo_skin_updater.h"
#include "gui_framework.h"
#include "config.h"
#include "skin_changer.h"
#include "aimbot.h"
#include "misc.h"
#include "esp.h"
#include "chams.h"
#include "standalone_recoil.h"
#include "insta_lock.h"
#include "triggerbot.h"
#include "menu_gui.h"
#include "new_menu.h"
#include "math.h"

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "ntdll.lib")
