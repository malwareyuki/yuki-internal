#include <stdexcept>
#include "memory_hook.h"
#include <Windows.h> // VirtualProtect, VirtualFree, VirtualAlloc


#ifdef _WIN64
#include "hde64.h"
#include "table64.h"
#else
#include "hde32.h"
#include "table32.h"
#endif



void vprotect_page(const uint64_t addr, const size_t size, const uint8_t fullperm, uint32_t *old)
{
	VirtualProtect((LPVOID)addr, size, fullperm ? PAGE_EXECUTE_READWRITE : *old, (LPDWORD)old);
}

const uint64_t valloc(const size_t size)
{
	return (uint64_t)VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

void vfree(const uint64_t addr)
{
	VirtualFree((LPVOID)addr, 0, MEM_FREE);
}

#ifdef _WIN64
const uint64_t valloc_near_page(const uint64_t addr, const size_t size)
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	MEMORY_BASIC_INFORMATION mbi;
	if (!VirtualQuery((LPCVOID)addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) || mbi.State & MEM_FREE)
		return (const uint64_t)VirtualAlloc((LPVOID)((uint64_t)mbi.BaseAddress + ((uint64_t)mbi.BaseAddress % si.dwAllocationGranularity)), size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE); /* currently free memory, lol. */
	

	size_t page_size = mbi.RegionSize;

	uint64_t page_next = (uint64_t)mbi.BaseAddress + page_size;
	uint64_t page_prev = (uint64_t)mbi.BaseAddress - page_size;

	while (page_next && page_prev)
	{
		if (page_prev && VirtualQuery((LPCVOID)page_prev, &mbi, sizeof(MEMORY_BASIC_INFORMATION)))
		{
			if (mbi.State == MEM_FREE) // Sweet, found a page we can allocate on.
				return (const uint64_t)VirtualAlloc((LPVOID)(page_prev + (page_prev % si.dwAllocationGranularity)), size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

			page_prev -= mbi.RegionSize;
		}
		if (addr - (uint64_t)mbi.BaseAddress >= MAXDWORD)
			page_prev = 0; // can't go any further :/

		if (page_next && VirtualQuery((LPCVOID)page_next, &mbi, sizeof(MEMORY_BASIC_INFORMATION)))
		{
			if (mbi.State == MEM_FREE)
				return (const uint64_t)VirtualAlloc((LPVOID)(page_next + (page_next % si.dwAllocationGranularity)), size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

			page_next += mbi.RegionSize;
		}
		if ((uint64_t)mbi.BaseAddress - addr >= MAXDWORD)
			page_next = 0; // can't go any further :/
	}
	return NULL;
}
#endif



std::list<memory_hook::memhook_context *> memory_hook::hook_list;



memory_hook::memhook_context *memory_hook::create_context(uint64_t target)
{
	memhook_context *hctx = (memhook_context *)malloc(sizeof(memhook_context));
	memset(hctx, 0, sizeof(memhook_context));

	hctx->target = target;
	hook_list.push_back(hctx);

	return hctx;
}

void memory_hook::destroy_context(uint64_t target)
{
	auto it = hook_list.begin();
	while (it != hook_list.end())
	{
		memhook_context *hctx = *it++;
		if (hctx->target == target)
		{
			/* this is our context, destroy it */
			hctx->destroy_hook();
			free(hctx);
			hook_list.erase(it);
			return;
		}
	}
}

void memory_hook::destroy_context(memhook_context *hctx)
{
	auto it = hook_list.begin();
	while (it != hook_list.end())
	{
		if ((*it++) == hctx)
		{
			hctx->destroy_hook();
			free(*it);
			hook_list.erase(it);
			return;
		}
	}
}



// Calculates how many no-op's are required for 'nsize' byte changes with instructions starting from 'addr'.
// 'fnops' will no-op 'fnops' amount of instructions (1 will no-op 1 non-required instruction and etc)
// returns the amount of bytes you will need to nop after replacing 'nsize' amount of bytes.
const size_t calculate_nops(uint64_t addr, const size_t nsize, size_t fnops = 0u)
{
	size_t nops = 0u;

	while (true)
	{
#ifdef _WIN64
		hde64s disasm;
		hde64_disasm((void *)addr, &disasm);
#else
		hde32s disasm;
		hde32_disasm((void *)addr, &disasm);
#endif
		addr += disasm.len;
		nops += disasm.len;

		if (nops > nsize)
		{
			if (fnops--)
				continue;
			break;
		}
	}
	nops -= nsize;
	return nops;
}

const size_t calculate_func_threshold(const uint64_t addr, const size_t nsize)
{
//#ifdef _WIN64
//	hde64s disasm;
//	hde64_disasm((void *)addr, &disasm);
//#else
//	hde32s disasm;
//	hde32_disasm((void *)addr, &disasm);
//#endif
//	if (disasm.opcode == 0xE9 && disasm.flags & C_REL32) /* Why would I need this again? */
//	{
//		if (disasm.imm.imm32 >)
//	}

	uint64_t end = addr;
	while (end - addr > nsize)
	{
	#ifdef _WIN64
		hde64s disasm;
		hde64_disasm((void *)end, &disasm);
	#else
		hde32s disasm;
		hde32_disasm((void *)end, &disasm);
	#endif
		switch (disasm.opcode)
		{
			case 0xC3: return end - addr;
			case 0xCB: return end - addr;
			case 0xC2: return end - addr;
			case 0xCA: return end - addr;
			default: break;
		}

		end += disasm.len;
	}
	return end - addr;
}

const uint64_t follow_hooks(const uint64_t addr, const size_t nsize, bool *hooked)
{
	uint64_t at = addr;
	while (at - addr < nsize)
	{
		hde64s disasm;
		hde64_disasm((void *)at, &disasm);

		switch (disasm.opcode)
		{
			case 0xE9: // jmp near, possibly hooked
			{
				if (disasm.flags & F_IMM32)
				{
					// Yeah it's most likely hooked
					*hooked = true;
					return at + (int32_t)disasm.imm.imm32 + disasm.len;
				}
				break;
			}
			case 0xFF: // jmp near
			{
				if (disasm.flags & F_IMM64 > 0)
				{
					// We'll consider this hooked
					*hooked = true;
					return disasm.imm.imm64;
				}
				break;
			}
		}

		at += disasm.len;
	}

	// If we make it to here, we're not hooked (well, dunno about that but we can hook at least with *nsize* amount of bytes to write)
	*hooked = false;
	return addr;
}


#ifdef _WIN64
const uint64_t memory_hook::memhook_context::create_hook(uint64_t detour, const bool follow)
{
	uint8_t relay_stub[] = {
			0x49, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//mov r10, addr
			0x41, 0xFF, 0xE2									//jmp r10
	};

	uint8_t rel_jmp_patch[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };			// jmp rel32

	uint8_t sendback_stub[] = {
			0x49, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//mov r10, addr
			0x41, 0xFF, 0xE2									//jmp r10
	};

	if (follow)
	{
		bool hooked = false;
		do
		{
			this->target = follow_hooks(this->target, detour - this->target - 5 > MAXDWORD ? sizeof(relay_stub) : sizeof(rel_jmp_patch), &hooked);
		} while (hooked && this->target);
		if (!this->target)
			return NULL;
	}


	uint64_t rel_jmp_off = detour - this->target - 5;
	uint8_t req_relay = rel_jmp_off > MAXDWORD;
	uint8_t embed_relay = FALSE;

	if (req_relay)
	{
		uint64_t relay_addr = detour;
		// Copy our address into the relay stub
		memcpy((void *)(relay_stub + 2), (void *)(((char *)&relay_addr) + sizeof(uint64_t) - sizeof(uint64_t)), sizeof(uint64_t));
		
		if (!calculate_func_threshold(this->target, sizeof(relay_stub)))
		{
			// Can't embed relay stub
			relay_addr = valloc_near_page(this->target, sizeof(relay_stub));
			if (!relay_addr) // Forced relay embed
				embed_relay = TRUE;
			else
			{
				// Mark hook as RELAYHOOK being that it has a relay function allocated somewhere near target (needs to be free'd).
				this->flags |= MEMHOOK_IS_RELAYHOOK;

				// Write our stub for later use
				memcpy((void *)relay_addr, relay_stub, sizeof(relay_stub));

				detour = relay_addr;
				rel_jmp_off = detour - this->target - 5;
				//embed_relay = FALSE; // Not needed, it is by default false.
			}
		}
		else
		{
			detour = relay_addr;
			embed_relay = TRUE;
		}
	}
	
	// Obviously, this is a hook.
	this->flags |= MEMHOOK_IS_HOOK;
	
	if (embed_relay || rel_jmp_off > MAXDWORD)
	{
		// We can embed the relay function, saves a page of memory for maybe a future hook.
		const size_t nops = calculate_nops(this->target, sizeof(relay_stub));

		// Copy hooking memory (but do not patch it to target)
		this->hook_size = sizeof(relay_stub) + nops;
		this->thk_mem = (uint8_t *)malloc(this->hook_size);
		memcpy(this->thk_mem, relay_stub, sizeof(relay_stub));

		// Implement required nops (no-operation instructions) to make the instructions original after hooking.
		memset(this->thk_mem + sizeof(relay_stub), 0x90, nops);
	}
	else
	{
		// Use relative jmp for hooking, function is out of threshold.
		const size_t nops = calculate_nops(this->target, sizeof(rel_jmp_patch));

		// Create our hook (but do not patch it into target yet)
		memcpy(rel_jmp_patch + 1, &rel_jmp_off, sizeof(uint32_t)); // Size mismatch, due to relative jmps having only 32-bit offset.

		this->hook_size = sizeof(rel_jmp_patch) + nops;
		this->thk_mem = (uint8_t *)malloc(this->hook_size);
		memcpy(this->thk_mem, rel_jmp_patch, sizeof(rel_jmp_patch));

		// Implement required nops (no-operation instructions) to make the instructions original after hooking.
		memset(this->thk_mem + sizeof(rel_jmp_patch), 0x90, nops);
	}

	// Get original function bytes
	this->old_mem = (uint8_t *)malloc(this->hook_size);

	uint32_t old;
	vprotect_page(this->target, this->hook_size, TRUE, &old);
	memcpy(this->old_mem, (void *)this->target, this->hook_size);
	vprotect_page(this->target, this->hook_size, FALSE, &old);
	
	
	// Create jmp back function (re-create original function)
	uint64_t endhook = this->target + this->hook_size;
	memcpy((void *)(sendback_stub + 2), &endhook, sizeof(uint64_t));

	// Allocate & copy memory to jmp back function
	this->jmpbck = valloc(this->hook_size + sizeof(sendback_stub));
	memcpy((void *)this->jmpbck, this->old_mem, this->hook_size);
	memcpy((void *)(this->jmpbck + this->hook_size), sendback_stub, sizeof(sendback_stub));
	
	// Update detour function (in struct)
	this->detour = detour;
	return this->jmpbck;
}
#else
const uint64_t memory_hook::memhook_context::create_hook(const uint64_t detour)
{
#pragma error("TODO: Implement 32-bit hooking. Lol, I am so lazy fuck you.")
	return NULL;
}
#endif


void memory_hook::memhook_context::destroy_hook()
{
	if (!this->detour)
		return; // Something is wrong maybe, lol idk. Probably no longer a valid hook

	// Just free everything, and restore bytes
	if (this->old_mem) // Means hook was enabled, so just yeetaledeet I'm not here
	{
		uint32_t old;
		vprotect_page(this->target, this->hook_size, TRUE, &old);
		memcpy((void *)this->target, this->old_mem, this->hook_size);
		vprotect_page(this->target, this->hook_size, FALSE, &old);
		free(this->old_mem);
	}
	if (this->jmpbck)
		vfree(this->jmpbck);
	if (this->thk_mem)
		free(this->thk_mem);
	if (this->flags & MEMHOOK_IS_RELAYHOOK && this->detour)
		vfree(this->detour);
}

void memory_hook::memhook_context::enable_hook()
{
	if (this->flags & MEMHOOK_IS_ENABLED)
		return;
	if (!this->thk_mem)
		return;

	uint32_t old = 0;
	vprotect_page(this->target, this->hook_size, TRUE, &old);
	memcpy((void *)this->target, this->thk_mem, this->hook_size);
	vprotect_page(this->target, this->hook_size, FALSE, &old);
	free(this->thk_mem);
	this->thk_mem = nullptr;
	this->flags |= MEMHOOK_IS_ENABLED;
}

void memory_hook::memhook_context::disable_hook()
{
	if (!(this->flags & MEMHOOK_IS_ENABLED))
		return;
	if (this->thk_mem)
		return;

	uint32_t old;
	vprotect_page(this->target, this->hook_size, TRUE, &old);
	this->thk_mem = (uint8_t *)malloc(this->hook_size);
	memcpy((void *)this->thk_mem, (void *)this->target, this->hook_size);
	memcpy((void *)this->target, this->old_mem, this->hook_size);
	vprotect_page(this->target, this->hook_size, FALSE, &old);
	this->flags &= ~MEMHOOK_IS_ENABLED;
}