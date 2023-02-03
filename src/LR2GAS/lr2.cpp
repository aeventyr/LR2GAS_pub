#include <Windows.h>

#include "lr2.h"

using namespace LR2;

void LR2::initOffsets()
{
    OSVERSIONINFOEXW osInfo;
    NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
    *(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
    if (RtlGetVersion != NULL)
    {
        osInfo.dwOSVersionInfoSize = sizeof(osInfo);
        RtlGetVersion(&osInfo);
    }
    if (osInfo.dwMajorVersion == 7)
    {
        bmsNotesNum = (int*)0xCC27C;
        bmsTotal = (int*)0xCC28C;

        gaugeType = (GAUGE_TYPE*)0xEF840;
        battleType = (BATTLE_TYPE*)0xEF884;
        selectPressed = (char*)0x10F5D1;
        gaugeValue = (double*)0x187200;
        gaugeValueDisp = (double*)0x187210;
        totalNotes = (int*)0x187278;
        runningNotes = (int*)0x1872A4;
        resultLamp = (LAMP_TYPE*)0x1872A8;
        resultGraph = (int*)0x1873F4;
        replayData = (ReplayCmd**)0x18F170;
        replayIdx = (int*)0x18F178;
        replayStatus = (int*)0x18F17C;
        gbattleFlag = (char*)0x18F561;
        expertType = (int*)0x18F688;
    }
    else if (osInfo.dwMajorVersion == 10)
    {
        bmsNotesNum = (int*)0xDC27C;
        bmsTotal = (int*)0xDC28C;

        gaugeType = (GAUGE_TYPE*)0xFF840;
        battleType = (BATTLE_TYPE*)0xFF884;
        selectPressed = (char*)0x11F5D1;
        gaugeValue = (double*)0x197200;
        gaugeValueDisp = (double*)0x197210;
        totalNotes = (int*)0x197278;
        runningNotes = (int*)0x1972A4;
        resultLamp = (LAMP_TYPE*)0x1972A8;
        resultGraph = (int*)0x1973F4;
        replayData = (ReplayCmd**)0x19F170;
        replayIdx = (int*)0x19F178;
        replayStatus = (int*)0x19F17C;
        gbattleFlag = (char*)0x19F561;
        expertType = (int*)0x19F688;
    }
    else
    {
        //MessageBox(NULL, "You need at least Windows 10.", "Version Not Supported", MB_OK);
        bmsNotesNum = (int*)0xDC27C;
        bmsTotal = (int*)0xDC28C;

        gaugeType = (GAUGE_TYPE*)0xFF840;
        battleType = (BATTLE_TYPE*)0xFF884;
        selectPressed = (char*)0x11F5D1;
        gaugeValue = (double*)0x197200;
        gaugeValueDisp = (double*)0x197210;
        totalNotes = (int*)0x197278;
        runningNotes = (int*)0x1972A4;
        resultLamp = (LAMP_TYPE*)0x1972A8;
        resultGraph = (int*)0x1973F4;
        replayData = (ReplayCmd**)0x19F170;
        replayIdx = (int*)0x19F178;
        replayStatus = (int*)0x19F17C;
        gbattleFlag = (char*)0x19F561;
        expertType = (int*)0x19F688;
    }

    pfAppendReplay = (int(__cdecl*) (int*, int, char, short))0x4C0C00;
}