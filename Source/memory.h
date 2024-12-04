#pragma once

extern "C" long nt_read_virtual_memory(HANDLE process, void* address, void* buffer, std::size_t number_of_bytes_to_read, std::size_t * number_of_bytes_readed);
extern "C" long nt_write_virtual_memory(HANDLE process, void* address, void* buffer, std::size_t number_of_bytes_to_write, std::size_t * number_of_bytes_written);

static bool read_virtual_memory(std::uintptr_t address, std::uint8_t* buffer, std::size_t size) {
    std::size_t number_of_bytes_readed = {};
    return nt_read_virtual_memory(HANDLE(-1), reinterpret_cast<void*>(address), buffer, size, &number_of_bytes_readed) == 0;
}

static bool read_virtual_memory_ex(void* address, std::uint8_t* buffer, std::size_t size) {
    std::size_t number_of_bytes_readed = {};
    return nt_read_virtual_memory(HANDLE(-1), address, buffer, size, &number_of_bytes_readed) == 0;
}

static bool write_virtual_memory(std::uintptr_t address, std::uint8_t* buffer, std::size_t size) {
    std::size_t number_of_bytes_written = {};
    return nt_write_virtual_memory(HANDLE(-1), reinterpret_cast<void*>(address), buffer, size, &number_of_bytes_written) == 0;
}

template <typename type = uobject*> static type read(std::uintptr_t address) {
    std::uint8_t buffer[sizeof(type)] = {};
    if (!read_virtual_memory(address, buffer, sizeof(type)))
        return {};
    return *reinterpret_cast<type*>(buffer);
}
//
//template <typename T = uobject*> T read(std::uintptr_t Address) {
//    return reinterpret_cast<T(*)(std::uintptr_t)>(base + 0x1AA9D80)(Address - 0x8);
//}

template <typename type> static bool write(std::uintptr_t address, type value) {
    return write_virtual_memory(address, reinterpret_cast<uint8_t*>(&value), sizeof(type));
}

