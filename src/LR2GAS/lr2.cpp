#include <Windows.h>

#include "lr2.h"

using namespace LR2;

// FIXME: hook into the main loop, get stack pointer from there. Current way can and will break on OS updates.
static unsigned long long get_stack_offset()
{
    HMODULE ntdll = GetModuleHandle("ntdll.dll");
    if (!ntdll)
      return 0; // wtf bro

    if (static_cast<void *>(GetProcAddress(ntdll, "wine_get_version")) != nullptr)
        return 0xA30000; // Wine 10.0

    OSVERSIONINFOEXW osInfo{};
    NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
    *(FARPROC*)&RtlGetVersion = GetProcAddress(ntdll, "RtlGetVersion");
    if (RtlGetVersion != NULL)
    {
        osInfo.dwOSVersionInfoSize = sizeof(osInfo);
        RtlGetVersion(&osInfo);
    }

    if (osInfo.dwMajorVersion >= 10)
        return 0x10000;

    return 0;
}

void LR2::initOffsets()
{
    const unsigned long long stack_offset = get_stack_offset();

    bmsNotesNum = (int *)(stack_offset + 0xCC27C);
    bmsTotal = (int *)(stack_offset + 0xCC28C);

    gaugeType = (GAUGE_TYPE *)(stack_offset + 0xEF840);
    battleType = (BATTLE_TYPE *)(stack_offset + 0xEF884);
    selectPressedP1 = (char *)(stack_offset + 0x10F5D1);
    selectPressedP2 = (char *)(stack_offset + 0x10F5E5);
    gaugeValue = (double *)(stack_offset + 0x187200);
    gaugeValueDisp = (double *)(stack_offset + 0x187210);
    totalNotes = (int *)(stack_offset + 0x187278);
    runningNotes = (int *)(stack_offset + 0x1872A4);
    resultLamp = (LAMP_TYPE *)(stack_offset + 0x1872A8);
    resultGraph = (int *)(stack_offset + 0x1873F4);
    replayData = (ReplayCmd **)(stack_offset + 0x18F170);
    replayIdx = (int *)(stack_offset + 0x18F178);
    replayStatus = (int *)(stack_offset + 0x18F17C);
    gbattleFlag = (char *)(stack_offset + 0x18F561);
    expertType = (int *)(stack_offset + 0x18F688);

    pfAppendReplay = (int(__cdecl*) (int*, int, char, short))0x4C0C00;
}
