#pragma once

#include "lr2.h"

using namespace LR2;

class Gauge
{
    GAUGE_TYPE gaugeType = (GAUGE_TYPE) -1;
    double gaugeValue = 0;
    double judgeGaugeIncrements[JUDGE_COUNT] = { 0 };
    bool isSurvival = true;
    bool failed = false;

    int graphArrIdx = 0;

public:
    int graph[GRAPH_SAMPLE_COUNT] = { 0 };

    Gauge(GAUGE_TYPE gaugeType);

    void onJudge(int judge);
    void recordGraphSample();
    void recordGraphSample(int);
    void fillGraph();

    GAUGE_TYPE getType();
    double getValue();
    bool validForDisplay();
    
};