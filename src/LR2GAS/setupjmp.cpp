#include "setupjmp.h"

void SetupHooks(std::vector<SafetyHookMid>& hooks)
{
    hooks.push_back(safetyhook::create_mid(0x4B57C6, onParseChart));
    hooks.push_back(safetyhook::create_mid(0x42D366, onInitPlay));
    hooks.push_back(safetyhook::create_mid(0x4AD247, onInitRetry));
    hooks.push_back(safetyhook::create_mid(0x406308, onJudgeNote));
    hooks.push_back(safetyhook::create_mid(0x406646, onJudgeMine));
    hooks.push_back(safetyhook::create_mid(0x405C45, onRecordGraphSample));
    hooks.push_back(safetyhook::create_mid(0x4344AF, onEnterResult));
    hooks.push_back(safetyhook::create_mid(0x409310, onRefreshResult));
    hooks.push_back(safetyhook::create_mid(0x4325FB, onEnterSongSelect));
    SaveReplay = safetyhook::create_inline(0x4C09E0, onSaveReplay);
}