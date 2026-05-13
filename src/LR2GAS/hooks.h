#pragma once

#include <vector>

#include "logger.h"
#include "LR2Mem/safetyhook/safetyhook.hpp"
#include <LR2Mem/LR2Typedefs.hpp>

void onParseChart(SafetyHookContext& ctx);
void onInitPlay(SafetyHookContext& ctx);
void onInitRetry(SafetyHookContext& ctx);
void onJudgeMine(SafetyHookContext& ctx);
void onJudgeNote(SafetyHookContext& ctx);
void onRecordGraphSample(SafetyHookContext& ctx);
void onEnterResult(SafetyHookContext& ctx);
void onRefreshResult(SafetyHookContext& ctx);
void onEnterSongSelect(SafetyHookContext& ctx);

inline safetyhook::InlineHook SaveReplay;
int __cdecl onSaveReplay(LR2::REPLAY* rp, LR2::CSTR songMD5, LR2::CSTR localID);
