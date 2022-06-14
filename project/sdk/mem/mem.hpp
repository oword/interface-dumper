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

    // Signatures are cached internally. If a byte must be skipped, emplace 0xFF.
    void* sig_scan(const std::wstring& module, const std::vector<BYTE>& signature);
}