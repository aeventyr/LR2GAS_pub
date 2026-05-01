#pragma once

#include <vector>

#include "logger.h"
#include "LR2Mem/safetyhook/safetyhook.hpp"

void onParseChart(SafetyHookContext& ctx);
void onInitPlay(SafetyHookContext& ctx);
void onInitRetry(SafetyHookContext& ctx);
void onJudgeMine(SafetyHookContext& ctx);
void onJudgeNote(SafetyHookContext& ctx);
void onRecordGraphSample(SafetyHookContext& ctx);
void onEnterResult(SafetyHookContext& ctx);
void onRefreshResult(SafetyHookContext& ctx);
void onEnterSongSelect(SafetyHookContext& ctx);
