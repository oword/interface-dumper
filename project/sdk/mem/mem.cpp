#include <cwctype>
#include <map>
#include <algorithm>
#include <utility>
#include "mem.hpp"
#include "nt.hpp"
#include "../dependencies/crc32.hpp"

#define CRC_HASH(vector) crc32<IEEE8023_CRC32_POLYNOMIAL>(0, vector.begin(), vector.end())

[[nodiscard]]
void* sdk::get_module_base(std::wstring name)
{
    nt::PLDR_DATA node{get_ldr_head()};

    std::transform(name.begin(), name.end(), name.begin(), std::towlower);
    do
    {
        if(node->DllBase)
        {
            std::wstring buf{node->BaseDllName.Buffer};

            std::transform(buf.begin(), buf.end(), buf.begin(), std::towlower);
            if(name == buf) return node->DllBase;
        }

        node = CONTAINING_RECORD(node->InMemoryOrderLinks.Flink, nt::LDR_DATA, InMemoryOrderLinks);
    } while (node != get_ldr_head());

    return nullptr;
}

[[nodiscard]]
void* sdk::get_module_export(const std::wstring& module, std::string function)
{
    void* module_base{sdk::get_module_base(module)};
    nt::PIMAGE_EXPORT_DIRECTORY export_directory{get_export_directory(module_base)};
    ULONG* export_address_table{CALC_ABSOLUTE(module_base, export_directory->AddressOfFunctions, ULONG*)};
    ULONG* export_name_table{CALC_ABSOLUTE(module_base, export_directory->AddressOfNames, ULONG*)};
    USHORT* export_ordinal_table{CALC_ABSOLUTE(module_base, export_directory->AddressOfNameOrdinals, USHORT*)};

    if(!export_directory->AddressOfFunctions) return nullptr;

    std::transform(function.begin(), function.end(), function.begin(), std::tolower);

    for(size_t i{}; i < export_directory->NumberOfNames; i++)
    {
        std::string buf{CALC_ABSOLUTE(module_base, export_name_table[i], const char*)};

        std::transform(buf.begin(), buf.end(), buf.begin(), std::tolower);
        if(function == buf)
        {
            DWORD ordinal{export_ordinal_table[i]};

            return CALC_ABSOLUTE(module_base, export_address_table[ordinal], void*);
        }
    }

    return nullptr;
}

void* sdk::find_byte_ref(void* begin, BYTE byte)
{
    BYTE* buf{reinterpret_cast<BYTE*>(begin)};

    while(true)
    {
        if(*buf == byte) return buf;
        else ++buf;
    }
}

bool sdk::byte_cmp(void* address, const std::vector<BYTE>& signature)
{
    for(size_t i{}; i < signature.size(); i++)
    {
        if(reinterpret_cast<BYTE*>(address)[i] == signature[i])
        {
            if(i == signature.size()-1)
                return true;
        }
        else return false;
    }

    return false;
}