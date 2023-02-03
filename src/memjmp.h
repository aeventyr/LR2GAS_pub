#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstring>

void __cdecl makeJmp(void* pAddress, void* dwDest, DWORD dwLen);
void __cdecl makeDetour(void* pAddress, void* dwDest, DWORD dwLen);