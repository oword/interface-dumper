#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
#include <vector>

#define CALC_ABSOLUTE(base, offset, type) ((type)((std::ptrdiff_t)base + (std::ptrdiff_t)offset))
#define CALC_REL(address, base, type) (((type)((std::ptrdiff_t)address - (std::ptrdiff_t)base)))

namespace sdk
{
    [[nodiscard]]
    void* get_module_base(std::wstring name);

    [[nodiscard]]
    void* get_module_export(const std::wstring& module, std::string function);

    //Scans for the first instance of "byte", starting at "begin".
    [[nodiscard]]
    void* find_byte_ref(void* begin, BYTE byte);

    //Scans address against signature, returns true if all bytes match.
    [[nodiscard]]
    bool byte_cmp(void* address, const std::vector<BYTE>& signature);
}