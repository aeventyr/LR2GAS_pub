#include "memjmp.h"

const unsigned int CALL_SIZE = 5;
const unsigned int JMP_SIZE = 5;
const BYTE CALL = 0xE8;
const BYTE JMP = 0xE9;
const BYTE NOP = 0x90;

// overwrite orig insn with JMP to dwDest 
void makeJmp(void* pAddress, void* dwDest, DWORD dwLen)
{
    DWORD dwOldProtect, dwBkup, dwRelAddr;
    VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);

    // clear source and overwrite with JMP to gateway
    std::memset(pAddress, NOP, dwLen);
    // write JMP()
    dwRelAddr = (DWORD)((BYTE*)dwDest - (BYTE*)pAddress) - JMP_SIZE;
    *(BYTE*)pAddress = JMP;
    *(DWORD*)((BYTE*)pAddress + 0x1) = dwRelAddr;

    VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);
}

// overwrite orig insn with JMP to a buffer that
// calls dwDest func, executes orig insn and jumps back
void makeDetour(void* pAddress, void* dwDest, DWORD dwLen)
{
    DWORD dwOldProtect, dwBkup, dwRelAddr;
    DWORD callRelAddr, jmpbackRelAddr;
    BOOL hResult = VirtualProtect(
        pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    if (hResult == NULL)
        return;

    // allocate buffer for CALL() + orig insn + JMP()
    void* gatewayAddr = VirtualAlloc(
        0, dwLen + CALL_SIZE + JMP_SIZE,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (gatewayAddr == nullptr)
        return;

    // write CALL() @gatewayAddr
    callRelAddr = (DWORD)dwDest - ((DWORD)gatewayAddr + CALL_SIZE);
    *(BYTE*)gatewayAddr = CALL;
    *(DWORD*)((BYTE*)gatewayAddr + 0x1) = callRelAddr;

    // in case of overwriting a existing JMP insn
    if (*(BYTE*)pAddress == JMP)
    {
        // write JMP with newRelAddr @gatewayAddr+5
        DWORD origDest = (DWORD)pAddress + *(DWORD*)((BYTE*)pAddress + 0x1) + CALL_SIZE;
        DWORD newRelAddr = origDest - ((DWORD)gatewayAddr + CALL_SIZE + JMP_SIZE);
        *((BYTE*)gatewayAddr + CALL_SIZE) = JMP;
        *(DWORD*)((BYTE*)gatewayAddr + CALL_SIZE + 0x1) = newRelAddr;

        // write JMP @gatewayAddr+10
        jmpbackRelAddr = (DWORD)pAddress - ((DWORD)gatewayAddr + CALL_SIZE + JMP_SIZE);
        *((BYTE*)gatewayAddr + CALL_SIZE + JMP_SIZE) = JMP;
        *(DWORD*)((BYTE*)gatewayAddr + CALL_SIZE + JMP_SIZE + 0x1) = jmpbackRelAddr;
    }
    else
    {
        // write original instruction @gatewayAddr+5
        std::memcpy((BYTE*)gatewayAddr + CALL_SIZE, pAddress, dwLen);

        // write JMP @gatewayAddr+5+len
        jmpbackRelAddr = (DWORD)pAddress - ((DWORD)gatewayAddr + CALL_SIZE + JMP_SIZE);
        *((BYTE*)gatewayAddr + CALL_SIZE + dwLen) = JMP;
        *(DWORD*)((BYTE*)gatewayAddr + CALL_SIZE + dwLen + 0x1) = jmpbackRelAddr;
    }

    // clear source and overwrite with JMP to gateway
    std::memset(pAddress, NOP, dwLen);
    // write JMP @source
    dwRelAddr = (DWORD)gatewayAddr - ((DWORD)pAddress + JMP_SIZE);
    *((BYTE*)pAddress) = JMP;
    *(DWORD*)((BYTE*)pAddress + 0x1) = dwRelAddr;

    VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);
}
