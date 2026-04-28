#include "setupjmp.h"

void SetupHooks(SafetyHookMid hooks[])
{
    int i = 0;
    hooks[i++] = safetyhook::create_mid((void*)0x4B57C6, onParseChart);
    hooks[i++] = safetyhook::create_mid((void*)0x42D366, onInitPlay);
    hooks[i++] = safetyhook::create_mid((void*)0x4AD247, onInitRetry);
    hooks[i++] = safetyhook::create_mid((void*)0x406308, onJudgeNote);
    hooks[i++] = safetyhook::create_mid((void*)0x405C45, onRecordGraphSample);
    hooks[i++] = safetyhook::create_mid((void*)0x4344AF, onEnterResult);
    hooks[i++] = safetyhook::create_mid((void*)0x409310, onRefreshResult);
    hooks[i++] = safetyhook::create_mid((void*)0x4325FB, onEnterSongSelect);
}