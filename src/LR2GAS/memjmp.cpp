#include "memjmp.h"

void __declspec(naked) asmPush()
{
    __asm
    {
        push esp
        push ebp
        push eax
        push ebx
        push ecx
        push edx
        push esi
        push edi
        pushfd
        nop
    }
}

void __declspec(naked) asmPop()
{
    __asm
    {
        popfd
        pop edi
        pop esi
        pop edx
        pop ecx
        pop ebx
        pop eax
        pop ebp
        pop esp
        nop
    }
}

const unsigned int ASM_SIZE = 9;
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

    BYTE* ptrw = (BYTE*)gatewayAddr;
    // write CALL() @gatewayAddr
    callRelAddr = (DWORD)dwDest - ((DWORD)gatewayAddr + CALL_SIZE);
    *ptrw++ = CALL;
    *(DWORD*)ptrw = callRelAddr;
    ptrw += 0x4;

    // in case of overwriting a existing JMP insn
    if (*(BYTE*)pAddress == JMP)
    {
        // write JMP with newRelAddr @gatewayAddr+5
        DWORD origDest = (DWORD)pAddress + *(DWORD*)((BYTE*)pAddress + 0x1) + CALL_SIZE;
        DWORD newRelAddr = origDest - ((DWORD)gatewayAddr + CALL_SIZE + JMP_SIZE);
        *ptrw++ = JMP;
        *(DWORD*)ptrw = newRelAddr;
        ptrw += 0x4;

        // write JMP @gatewayAddr+10
        jmpbackRelAddr = (DWORD)pAddress - ((DWORD)gatewayAddr + CALL_SIZE + JMP_SIZE);
        *ptrw++ = JMP;
        *(DWORD*)ptrw = jmpbackRelAddr;
        ptrw += 0x4;
    }
    else
    {
        // write original instruction @gatewayAddr+5
        std::memcpy((BYTE*)gatewayAddr + CALL_SIZE, pAddress, dwLen);
        ptrw = (BYTE*)gatewayAddr + CALL_SIZE + dwLen;

        // write JMP @gatewayAddr+5+len
        jmpbackRelAddr = (DWORD)pAddress - ((DWORD)gatewayAddr + CALL_SIZE + JMP_SIZE);
        *ptrw++ = JMP;
        *(DWORD*)ptrw = jmpbackRelAddr;
        ptrw += 0x4;
    }

    // clear source and overwrite with JMP to gateway
    std::memset(pAddress, NOP, dwLen);
    // write JMP @source
    dwRelAddr = (DWORD)gatewayAddr - ((DWORD)pAddress + JMP_SIZE);
    *((BYTE*)pAddress) = JMP;
    *(DWORD*)((BYTE*)pAddress + 0x1) = dwRelAddr;

    VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);
}

// same as makeDetour but saves and restores registers
void makeDetourSafe(void* pAddress, void* dwDest, DWORD dwLen)
{
    DWORD dwOldProtect, dwBkup, dwRelAddr;
    DWORD callRelAddr, jmpbackRelAddr;
    BOOL hResult = VirtualProtect(
        pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    if (hResult == NULL)
        return;

    // allocate buffer for save + CALL() + orig insn + JMP() + restore
    void* gatewayAddr = VirtualAlloc(
        0, dwLen + CALL_SIZE + JMP_SIZE + ASM_SIZE + ASM_SIZE,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (gatewayAddr == nullptr)
        return;

    // write reg save @gatewayAddr
    BYTE* ptrw = (BYTE*)gatewayAddr;
    BYTE* ptrasm = (BYTE*)asmPush;
    while (*ptrasm != NOP)
        *ptrw++ = *ptrasm++;

    // write CALL() @gatewayAddr+asmsave
    callRelAddr = (DWORD)dwDest - ((DWORD)ptrw + CALL_SIZE);
    *ptrw++ = CALL;
    *(DWORD*)ptrw = callRelAddr;
    ptrw += 0x4;

    // in case of overwriting a existing JMP insn
    if ((*(BYTE*)pAddress == JMP || *(BYTE*)pAddress == CALL) && dwLen == 5)
    {
        // write reg restore 
        ptrasm = (BYTE*)asmPop;
        while (*ptrasm != NOP)
            *ptrw++ = *ptrasm++;

        // write JMP with newRelAddr @gatewayAddr+asmsave+5
        DWORD origDest = (DWORD)pAddress + *(DWORD*)((BYTE*)pAddress + 0x1) + CALL_SIZE;
        DWORD newRelAddr = origDest - ((DWORD)ptrw + CALL_SIZE);
        *ptrw++ = *(BYTE*)pAddress;
        *(DWORD*)ptrw = newRelAddr;
        ptrw += 0x4;

        // write JMP @gatewayAddr+asmsave+10
        jmpbackRelAddr = ((DWORD)pAddress + CALL_SIZE) - ((DWORD)ptrw + JMP_SIZE);
        *ptrw++ = JMP;
        *(DWORD*)ptrw = jmpbackRelAddr;
        ptrw += 0x4;
    }
    else
    {
        // write reg restore 
        ptrasm = (BYTE*)asmPop;
        while (*ptrasm != NOP)
            *ptrw++ = *ptrasm++;

        // write original instruction @gatewayAddr+asmsave+5
        std::memcpy(ptrw, pAddress, dwLen);
        ptrw += dwLen;

        // write JMP @gatewayAddr+asmsave+5+len
        jmpbackRelAddr = ((DWORD)pAddress + dwLen) - ((DWORD)ptrw + JMP_SIZE);
        *ptrw++ = JMP;
        *(DWORD*)ptrw = jmpbackRelAddr;
        ptrw += 0x4;
    }

    // clear source and overwrite with JMP to gateway
    std::memset(pAddress, NOP, dwLen);
    // write JMP @source
    dwRelAddr = (DWORD)gatewayAddr - ((DWORD)pAddress + JMP_SIZE);
    *((BYTE*)pAddress) = JMP;
    *(DWORD*)((BYTE*)pAddress + 0x1) = dwRelAddr;

    VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);
}
