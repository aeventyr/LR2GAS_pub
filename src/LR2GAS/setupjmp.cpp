#include "setupjmp.h"

void SetupJmps()
{
    makeDetour((void*)0x4B57C6, &onParseChart, 6);
    makeDetour((void*)0x42D366, &onInitPlay, 6);
    makeDetour((void*)0x4AD247, &onInitRetry, 6);
    makeDetour((void*)0x406308, &onJudgeNote, 7);
    makeDetour((void*)0x405C45, &onRecordGraphSample, 6);
    makeDetour((void*)0x4344AF, &onEnterResult, 10);
    makeDetour((void*)0x409310, &onRefreshResult, 11);
    makeDetour((void*)0x4325FB, &onEnterSongSelect, 5);
}