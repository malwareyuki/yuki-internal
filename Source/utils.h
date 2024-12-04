#pragma once
#include "includes.h"

bool is_bad_ptr(uintptr_t* ptr) {
	if (!ptr || (uint64_t)ptr > 0xffffffffffff)
		return true;

	/*__try
	{
		volatile auto result = *(uint64_t*)(ptr);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return true;
	}*/
	return false;
}

#define roundz(x,d) ((floor(((x)*pow(10,d))+.5))/pow(10,d))

namespace utils {
	namespace pattern {

		inline bool int_compare_pattern(char* szSource, const char* szPattern, const char* szMask)
		{
			for (; *szMask; ++szSource, ++szPattern, ++szMask)
				if (*szMask == 'x' && *szSource != *szPattern)
					return false;
			return true;
		}

		inline char* int_pattern_scan(char* pData, UINT_PTR RegionSize, const char* szPattern, const char* szMask, int Len)
		{
			for (UINT i = 0; i != RegionSize - Len; ++i, ++pData)
				if (int_compare_pattern(pData, szPattern, szMask))
					return pData;
			return nullptr;
		}

		int count = 0;

		inline char* pattern_scan(char* pStart, UINT_PTR RegionSize, const char* szPattern, const char* szMask)
		{
			char* pCurrent = pStart;
			auto Len = lstrlenA(szMask);


			while (pCurrent <= pStart + RegionSize - Len)
			{

				MEMORY_BASIC_INFORMATION MBI{ 0 };
				//if (!__query_virtual_memory(HANDLE(-1), reinterpret_cast<void*>(pCurrent), MemoryBasicInformation, &MBI, sizeof(MBI), nullptr))
				if (!IFH(VirtualQuery)(pCurrent, &MBI, sizeof(MEMORY_BASIC_INFORMATION))) // Replace? Detection maybe?
				{
					return nullptr;
				}

				if (MBI.State == MEM_COMMIT && !(MBI.Protect & PAGE_NOACCESS) && !(MBI.Protect & PAGE_GUARD))
				{
					if (pCurrent + MBI.RegionSize > pStart + RegionSize - Len)
						MBI.RegionSize = pStart + RegionSize - pCurrent + Len;


					char* Ret = int_pattern_scan(pCurrent, MBI.RegionSize, szPattern, szMask, Len);

					if (Ret)
						return Ret;

				}

				pCurrent += MBI.RegionSize;
			}

			return nullptr;
		}

	}

    static BOOL IsValidPointer(uintptr_t address)
    {
        if (!is_bad_ptr((uintptr_t*)address))
            return TRUE;
        else
            return FALSE;
    }

	bool replace(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}


}