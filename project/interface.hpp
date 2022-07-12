#pragma once
#include "sdk/mem/mem.hpp"
#include "sdk/mem/nt.hpp"

class interface_list
{
public: //typedef
    typedef void*(__cdecl* t_get_interface)();

public: //fields
    t_get_interface fn_get_interface;   //0x00
    const char* name;                   //0x4
    interface_list* next;               //0x8

public: //constructor
    interface_list() = delete;
};

inline interface_list* get_interface_list_head(const std::wstring& module)
{
    void* create_interface{sdk::get_module_export(module, "CreateInterface")};
    interface_list* return_val{};

    if(!create_interface) return nullptr;

    /*
     * The CreateInterface export can be one of two functions, a wrapper for CreateInterface or the fn itself,
     * 'signature' refers to the signature of the wrapper fn,
     * if it is the wrapped fn we must manually calculate the position of the fn.
     */
    if(sdk::byte_cmp(create_interface, {0x55, 0x8B, 0xEC, 0x5D, 0xE9}))
    {
        //Store create_interface in buffer, so we can perform arithmetic.
        std::ptrdiff_t buf{reinterpret_cast<std::ptrdiff_t>(create_interface)};

        //0xE9 ??? : jmp, real-create-interface
        buf = reinterpret_cast<std::ptrdiff_t>(sdk::find_byte_ref(create_interface, 0xE9));
        /*
         * we must calculate the absolute address of real-create-interface:
         * EIP + real-create-interface + 5 <- size of 0xE9 jmp instruction
         */
        buf = buf + *reinterpret_cast<std::ptrdiff_t*>(buf + 1) + 5;
        //Update create_interface to appropriate pointer.
        create_interface = reinterpret_cast<void*>(buf);
    }

    //0x8B '0x35' 0x??? : mov 'esi', *interface-list*
    return_val = reinterpret_cast<interface_list*>(sdk::find_byte_ref(create_interface, 0x35));
    //We must offset return_val by 1, the mov instruction points to a pointer to interface-list, so we must dereference it twice.
    return **reinterpret_cast<interface_list***>(reinterpret_cast<std::ptrdiff_t>(return_val) + 1 );
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