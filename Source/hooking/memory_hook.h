#ifndef MEMORY_HOOK_H
#define MEMORY_HOOK_H
#pragma once
#include <cstdint>
#include <list>
/*
	x86/x64 (32-bit and 64-bit compatible)
	Memory Hooking Api by Cyberhound#5672 (discord)

		It was a long long day. I have troubles hearing. I thought I saw a plane,
	but turns out it was just a dolphin in the sky, and I thought to myself, "Hell!
	I wish I could fly like a dolphin too." Then one day, I just woke up and flew away!


	I'm retarded, so enjoy this uh- artwork? of C++ madness episode 4 of season 99.
	C++ is just beautiful.


	Note:
		I did not use classes, because they give off RTTI and can be sigged/detectable that way,
	at least I assume so. Though, structs should be safe as they do not have names.
*/


namespace memory_hook
{
#define MEMHOOK_IS_ENABLED		((uint8_t)(1u << 0))
//#define MEMHOOK_IS_VFT			((uint8_t)(1u << 1)) //TODO: Implement VFT Hooking (swapping)
#ifdef _WIN64
#define MEMHOOK_IS_RELAYHOOK		((uint8_t)(1u << 7))
#endif
#define MEMHOOK_IS_HOOK			((uint8_t)(1u << 2))

	struct memhook_context
	{
		uint8_t flags; // Hooking data (what kind, enabled, x64/x32)

		uint64_t target; // The function to hook
		uint64_t detour; // The hooked function
		uint64_t jmpbck; // The address of the jmp back function (the "original" function)

		uint8_t *old_mem; // The storage of the replaced bytes
		uint8_t *thk_mem; // Temporary storage of the hook memory (for disable/enable function of hooking)
		size_t hook_size; // The size of the hook (replaced bytes in target function)


		const uint64_t create_hook(uint64_t detour, const bool follow = true);
		void destroy_hook();

		void enable_hook();
		void disable_hook();

		void toggle_hook(const bool value) { if (value) enable_hook(); else disable_hook(); }


		template<typename detour_T>
		inline const detour_T hook(const detour_T detour, const bool follow = true) { return (detour_T)create_hook((uint64_t)detour, follow); }

		template<typename target_T, typename detour_T>
		inline const target_T hook(const detour_T detour, const bool follow = true) { return (target_T)create_hook((uint64_t)detour, follow); }
	};


	extern std::list<memhook_context *> hook_list;


	memhook_context *create_context(uint64_t target);
	void destroy_context(uint64_t target);
	void destroy_context(memhook_context *hctx);


	template<typename target_T, typename detour_T>
	target_T create_hook(target_T target, detour_T detour) { return create_context((uint64_t)target)->hook((uint64_t)detour); }

	template<typename target_T>
	memhook_context *create_context(target_T target) { return create_context((uint64_t)target); }
}
#endif // MEMORY_HOOK_H