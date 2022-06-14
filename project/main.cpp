#include <Windows.h>
#include <iostream>
#include "interface.hpp"

DWORD WINAPI thread(HMODULE module)
{
    FILE* stream{};

    AllocConsole();
    freopen_s(&stream, "CONOUT$", "w", stdout);

    dump_interfaces();

    while(!(GetAsyncKeyState(VK_END) & 1));

    fclose(stream);
    FreeConsole();
    FreeLibraryAndExitThread(module, 0);
}

BOOL WINAPI DllMain(HMODULE handle, DWORD reason, void*)
{
    DisableThreadLibraryCalls(handle);

    if(reason == DLL_PROCESS_ATTACH)
    {
        if(!CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(thread), &handle, NULL, nullptr))
            return false;
    }

    return true;
}