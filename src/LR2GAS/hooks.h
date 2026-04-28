#pragma once

#include <vector>

#include "logger.h"
#include "safetyhook/safetyhook.hpp"

void onParseChart(SafetyHookContext& ctx);
void onInitPlay(SafetyHookContext& ctx);
void onInitRetry(SafetyHookContext& ctx);
void onJudgeNote(SafetyHookContext& ctx);
void onRecordGraphSample(SafetyHookContext& ctx);
void onEnterResult(SafetyHookContext& ctx);
void onRefreshResult(SafetyHookContext& ctx);
void onEnterSongSelect(SafetyHookContext& ctx);

