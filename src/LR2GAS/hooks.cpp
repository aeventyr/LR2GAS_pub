#include "hooks.h"
#include "lr2.h"
#include "gas.h"

using namespace LR2;

static bool gasBlocked()
{
    return (*battleType == BATTLE_BATTLE && !*gbattleFlag) ||
        *replayStatus == 2 ||
        *expertType != -1;
}

static bool canGas()
{
    return !GAS::gaugeArr.empty() && !gasBlocked();
}

static void handleJudge(JUDGE_TYPE judge, int mineDamage)
{
    // update gauge
    for (Gauge& g : GAS::gaugeArr)
        g.onJudge(judge, mineDamage);
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

void onParseChart(SafetyHookContext& ctx)
{
    GAS::notesNum = *(unsigned int*)(ctx.esp + 0xD4);
    // casts intentional
    GAS::bmsTotal = (double) *(unsigned int*)(ctx.esp + 0xE4);
}

void onInitPlay(SafetyHookContext& ctx)
{
    if (gasBlocked())
        return;

    // record gauge
    GAUGE_TYPE gauge = *gaugeType;
    GAS::initialGauge = gauge;

    console_log("play: gauge: %s\t", GAUGE_TYPE_STR[gauge]);
    console_log("notes: %d\ttotal: %d\n",
        GAS::notesNum, (int)GAS::bmsTotal);
    GAS::Init();
}

void onInitRetry(SafetyHookContext& ctx)
{
    if (!canGas())
        return;

    // restore gauge
    *gaugeType = (GAUGE_TYPE)GAS::initialGauge;

    console_log("retry: gauge: %s\t", GAUGE_TYPE_STR[*gaugeType]);
    console_log("notes: %d\ttotal: %d\n",
        GAS::notesNum, (int)GAS::bmsTotal);
    GAS::Init();
}

void onJudgeMine(SafetyHookContext& ctx)
{
    if (!canGas())
        return;

    JUDGE_TYPE judge = (JUDGE_TYPE) * (int*)(ctx.esp + 0xC);
    int mineDamage = *(int*)(ctx.esp + 0x1C);

    handleJudge(judge, mineDamage);
}

void onJudgeNote(SafetyHookContext& ctx)
{
    if (!canGas())
        return;

    // grab current judge & battle status
    JUDGE_TYPE judge = (JUDGE_TYPE) *(int*)(ctx.esp + 0x14);
    int isBattlePlayer = *(int*)(ctx.esp + 0x1C);

    if (!isBattlePlayer)
    {
        handleJudge(judge, 0);
    }
}

void onRecordGraphSample(SafetyHookContext& ctx)
{
    if (!canGas())
        return;

    int playerstatus = *(int*)(ctx.esp + 0x18);
    if (!playerstatus)
        return;

    for (Gauge& g : GAS::gaugeArr)
    {
        g.recordGraphSample();
    }
}

void onEnterResult(SafetyHookContext& ctx)
{
    if (!canGas())
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

void onRefreshResult(SafetyHookContext& ctx)
{
    if (!canGas())
        return;
    // cycle through gauges on pressing select
    if (*selectPressedP1 == 1 || *selectPressedP2 == 1)
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

void onEnterSongSelect(SafetyHookContext& ctx)
{
    // restore gauge
    if (GAS::initialGauge >= 0)
        *gaugeType = GAS::initialGauge;
}


