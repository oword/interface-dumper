#pragma once
#include "sdk/mem/mem.hpp"
#include "sdk/mem/nt.hpp"

class interface_list
{
public: //typedef
    typedef void*(__cdecl* t_get_interface)();

public: //fields
    t_get_interface fn_get_interface;
    const char* name;
    interface_list* next;

public: //constructor
    interface_list() = delete;
};

//This function can be optimized by manually calculating the position of interface_list rather than scanning for CreateInterface,
//the reason I didn't use the exported CreateInterface is because CreateInterface is a wrapper for the actual function.
inline interface_list* get_interface_list_head(const std::wstring& module)
{
    void* create_interface{};

    if(!sdk::get_module_export(module, "CreateInterface")) return nullptr;

    create_interface = sdk::sig_scan(module, {0x55, 0x8B, 0xEC, 0x56, 0x8B, 0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0x57, 0x85, 0xF6, 0x74, 0xFF, 0x8B, 0x7D, 0xFF, 0x8B, 0x46, 0xFF, 0x8B, 0xCF});

    if(!create_interface) return nullptr;

    return **CALC_ABSOLUTE(create_interface, 6, interface_list***);
}

inline void dump_interfaces()
{
    nt::PLDR_DATA node{get_ldr_head()};

    do
    {
        if(node->DllBase)
        {
            interface_list* list{get_interface_list_head(node->BaseDllName.Buffer)};

            if(list) std::wcout << "[" << node->BaseDllName.Buffer << ']' << '\n';

            while((list) && list->next)
            {
                std::cout << '\t' <<list->name << " -> relative-address -> " << CALC_REL(list->fn_get_interface(), node->DllBase, void*) << '\n';

                list = list->next;
            }
        }

        node = CONTAINING_RECORD(node->InMemoryOrderLinks.Flink, nt::LDR_DATA, InMemoryOrderLinks);
    } while (node != get_ldr_head());
}