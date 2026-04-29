#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <thread>

#include "setupjmp.h"
#include "lr2.h"

#include <LR2Mem/LR2Bindings.hpp>

#pragma comment(lib, "LR2Mem.lib")

void Setup(const HMODULE instance)
{
    std::vector<SafetyHookMid> hooks{};

#ifdef _CONSOLE
    FILE* f;
    BOOL hResult = AllocConsole();
    int fres = freopen_s(&f, "CONOUT$", "w", stderr);
    if (fres != 0)
    {
        goto UNLOAD;
    }
#endif

    LR2::Init();
    LR2::matchOffsets();
    SetupHooks(hooks);
    while (1)
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

UNLOAD:

#ifdef _CONSOLE
    fclose(f);
    FreeConsole();
#endif

    FreeLibraryAndExitThread(instance, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        const auto thread = CreateThread(
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE> (Setup),
            hModule,
            0,
            nullptr
        );

        if (thread) {
            CloseHandle(thread);
        }
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

