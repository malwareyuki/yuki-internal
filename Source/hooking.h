#pragma once

#include "includes.h"

typedef enum _MEMORY_INFORMATION_CLASS {
	MemoryBasicInformation
} MEMORY_INFORMATION_CLASS;

extern "C" NTSTATUS __query_virtual_memory(
	HANDLE p_handle,
	void* base_addr,
	MEMORY_INFORMATION_CLASS memory_info_class,
	void* mbi,
	std::size_t mbi_length,
	std::size_t * mbi_out_length
);

namespace pinguu {
	namespace vmt {
		class shadow {
		public:

			shadow() {}
			
			shadow(void* object) { this->initialize(object); }

			bool initialize(void* object, std::size_t size = 0) {
				this->object = object;
				if (this->object == nullptr) return false;

				this->object_vtable = *reinterpret_cast<std::uintptr_t**>(this->object);
				if (this->object_vtable == nullptr) return false;
				if (this->object_vtable == this->object_fake_vtable) return false;

				if (size == 0) {
					while (is_valid_ptr_value(this->object_vtable[size])) ++size;
					size *= sizeof(std::uintptr_t);
				}

				this->object_fake_vtable = reinterpret_cast<std::uintptr_t*>(VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE));
				if (this->object_fake_vtable == nullptr) return false;

				__movsb(reinterpret_cast<std::uint8_t*>(this->object_fake_vtable), reinterpret_cast<std::uint8_t*>(this->object_vtable), size);

				*reinterpret_cast<std::uintptr_t**>(this->object) = this->object_fake_vtable;

				return true;
			}

			void restore_table() {
				if (this->object == nullptr) return;
				if (this->object_vtable == nullptr) return;
				*reinterpret_cast<std::uintptr_t**>(this->object) = this->object_vtable;
			}

			template <typename function> function apply(std::uintptr_t address, std::size_t index) {
				if (!address)
					return function();
				
				if (const std::uintptr_t original = this->object_fake_vtable[index])
					if (this->object_fake_vtable[index] = address)
						return reinterpret_cast<function>(original);

				return function();
			}

			uint64_t get_address_from_index(std::size_t index) {
				return this->object_fake_vtable[index];
			}

		private:
			void* object = nullptr;
			std::uintptr_t* object_vtable = nullptr;
			std::uintptr_t* object_fake_vtable = nullptr;

		public:
			static inline std::uintptr_t rwx_cave = std::uintptr_t();
			static inline std::int32_t total_function = std::int32_t();

			bool is_valid_ptr_value(std::uintptr_t pointer) {
				return pointer && (pointer >= 0x10000) && (pointer < 0xF000000000000);
			}

			static std::uintptr_t bind_function_to_rwx_cave(void* function) {
				if (shadow::rwx_cave == 0)
					return std::uintptr_t();

				std::array<std::uint8_t, 15> stub = { 0xFF, 0x25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xC3 };
				*reinterpret_cast<void**>(&stub[6]) = function;

				const std::uintptr_t address = (shadow::rwx_cave + 0x80) + (shadow::total_function * stub.size());

				__movsb(reinterpret_cast<std::uint8_t*>(address), stub.data(), stub.size());

				++shadow::total_function;

				return address;
			}

			static void unbind_all_functions()
			{
				if (shadow::total_function == 0 || shadow::rwx_cave == 0)
					return;

				std::array<std::uint8_t, 15> stub = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

				for (int x = 0; x != shadow::total_function; x++)
				{
					const std::uintptr_t address = (shadow::rwx_cave + 0x80) + (x * stub.size());
					__movsb(reinterpret_cast<std::uint8_t*>(address), stub.data(), stub.size());
				}
			}

			static std::uintptr_t find_whitelisted_rwx_cave() {
				MEMORY_BASIC_INFORMATION mbi = {};
				std::uintptr_t address = std::uintptr_t();

				while (__query_virtual_memory(HANDLE(-1), reinterpret_cast<void*>(address), MemoryBasicInformation, &mbi, sizeof(mbi), nullptr) == 0) {
					if (address && mbi.Protect == PAGE_EXECUTE_READWRITE && mbi.AllocationProtect == PAGE_EXECUTE_READWRITE && mbi.Type == MEM_PRIVATE && mbi.State == MEM_COMMIT) {
						std::uint8_t* pattern_address = reinterpret_cast<std::uint8_t*>(address);

						bool is_empty_end = true;
						for (std::size_t index = 100; index < mbi.RegionSize; index++)
							if (!(pattern_address[index] == 0ui8))
								if (is_empty_end = !is_empty_end; is_empty_end == false)
									break;

						if (is_empty_end && pattern_address[0] == 0x48 && pattern_address[1] == 0x89 && pattern_address[2] == 0x5C && pattern_address[3] == 0x24)
							return address;
					}

					address += mbi.RegionSize;
				}

				return address;
			}
		};
	}
}