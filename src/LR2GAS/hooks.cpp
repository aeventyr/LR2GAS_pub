#include "hooks.h"
#include "lr2.h"
#include "gas.h"

using namespace LR2;

bool getCond()
{
    return (*battleType == BATTLE_BATTLE ||
        *gbattleFlag ||
        *replayStatus == 2 ||
        *expertType != -1);
}

void onInitPlay()
{
    // record gauge
    GAUGE_TYPE gauge = *gaugeType;
    GAS::initialGauge = gauge;

    if (getCond())
        return;
    GAS::notesNum = *bmsNotesNum;
    GAS::bmsTotal = *bmsTotal;

    console_log("play: gauge: %s\t", GAUGE_TYPE_STR[gauge]);
    console_log("notes: %d\ttotal: %d\n", 
        GAS::notesNum, (int)GAS::bmsTotal);
    GAS::Init();
}

void onInitRetry()
{
    // restore gauge
    *gaugeType = (GAUGE_TYPE)GAS::initialGauge;

    if (getCond())
        return;
    GAS::notesNum = *bmsNotesNum;
    GAS::bmsTotal = *bmsTotal;

    console_log("retry: gauge: %s\t", GAUGE_TYPE_STR[*gaugeType]);
    console_log("notes: %d\ttotal: %d\n", 
        GAS::notesNum, (int)GAS::bmsTotal);
    GAS::Init();
}

void onJudgeNote()
{
    if (getCond())
        return;

    JUDGE_TYPE judge;
    int isBattlePlayer;
    __asm
    {
        // grab cur judge & battle cond
        PUSH[ebp + 0x1C]
        POP judge
        PUSH[ebp + 0x24]
        POP isBattlePlayer
    };
    if (!isBattlePlayer)
    {
        // update gauge
        for (Gauge& g : GAS::gaugeArr)
            g.onJudge(judge);
        // overwrite game gauge
        auto prevGauge = *gaugeType;
        Gauge& displayGauge = GAS::PickDisplay();
        *gaugeType = (GAUGE_TYPE)displayGauge.getType();
        *gaugeValue = displayGauge.getValue();

        if (*replayStatus == 1 &&
            *gaugeType != prevGauge)
        {
            // update replay gauge
            ReplayCmd temp = (*replayData)[*replayIdx - 1];
            pfAppendReplay((int*)replayData, temp.time, 101,
                *gaugeType);
            (*replayData)[*replayIdx - 2] = (*replayData)[*replayIdx - 1];
            (*replayData)[*replayIdx - 1] = temp;
        }

        console_log("judge:%d\tgauge:%s\t", 
            judge, GAUGE_TYPE_STR[displayGauge.getType()]);
        for (Gauge& g : GAS::gaugeArr)
        {
            console_log("%.2lf\t", g.getValue());
        }
        console_log("\n");
    }

}

void onRecordGraphSample()
{
    if (getCond())
        return;

    for (Gauge& g : GAS::gaugeArr)
    {
        g.recordGraphSample();
    }
}

void onEnterResult()
{
    if (getCond())
        return;

    // finish up graph
    for (Gauge& g : GAS::gaugeArr)
    {
        if (*runningNotes < *totalNotes)
            g.recordGraphSample(0);
        g.fillGraph();
    }
    // overwrite gauge val & graph
    Gauge& displayGauge = GAS::PickDisplay();
    *gaugeValueDisp = displayGauge.getValue();
    std::copy(displayGauge.graph, displayGauge.graph + GRAPH_SAMPLE_COUNT,
        resultGraph);
}

void onRefreshResult()
{
    if (getCond())
        return;
    // cycle through gauges on pressing select 
    if (*selectPressed == 1)
    {
        GAS::gaugeCycleIdx %= GAS::gaugeArr.size();
        Gauge& sel = GAS::gaugeArr[GAS::gaugeCycleIdx];
        *gaugeType = sel.getType();
        *gaugeValueDisp = sel.getValue();

        console_log("gauge:%s\t%lf\n",
            GAUGE_TYPE_STR[sel.getType()],
            sel.getValue());
        // overwrite graph
        std::copy(sel.graph, sel.graph + GRAPH_SAMPLE_COUNT,
            resultGraph);

        GAS::gaugeCycleIdx++;
    }
}

void onEnterSongSelect()
{
    // restore gauge
    if (GAS::initialGauge >= 0)
        *gaugeType = GAS::initialGauge;
}


